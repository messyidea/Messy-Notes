package main

import (
	"bytes"
	"fmt"
	"net"
	"os"
	"runtime"
	"strings"
	"time"
    "crypto/tls"
    "flag"
)

var (
    argPort string
)

type Server struct {
    Config   *tls.Config
    Listener *net.Listener
    Quit     chan bool
}

func NewServer() *Server {
    return &Server{
        Config:   nil,
        Listener: nil,
        Quit:     make(chan bool, 1),
    }
}

func (s *Server) Initialize() {
    cert, err := tls.LoadX509KeyPair("public.crt", "private.pem")
    if err != nil {
        panic(err)
    }
    s.Config = &tls.Config{
        Certificates: make([]tls.Certificate, 1),
    }
    s.Config.Certificates[0] = cert
    s.Config.BuildNameToCertificate()
}

func (s *Server) Close() {
    s.Quit <- true
    (*s.Listener).Close()
}



func Log(v ...interface{}) {
	// fmt.Fprintf(os.Stderr, time.Now())
	// fmt.Fprintln(os.Stderr, v...)
	msg := fmt.Sprintf("%v ", time.Now())[:23]
	pc, _, line, _ := runtime.Caller(1)
	fn := runtime.FuncForPC(pc)
	msg += fmt.Sprintf(" %v:%v ", fn.Name(), line)
	msg += fmt.Sprintf("%v ", runtime.NumGoroutine())
	msg += fmt.Sprint(v...)
	fmt.Fprintln(os.Stderr, msg)
}

const (
	ConnectResponse = "HTTP/1.1 200 Connection established\r\n\r\n"
)

type A struct {
	a, b int
}

func (self *A) foo(i int) {
	self.a += i
	*self = A{10, 20}
}
func (self *A) String() string {
	return fmt.Sprintf("A.a=%d A.b=%d", self.a, self.b)
}

type SConn struct {
	*net.TCPConn
	query string
}

func SConn_new(con *net.TCPConn) *SConn {
	return &SConn{con, ""}
}

func main() {
	//fmt.Println("Hello World!")
	// fmt.Printf("This %d", 200)
	// nslookup("google.com")
	// a := A{1,2}
	// fmt.Println(a)
	// a.foo(3)
	// fmt.Println(a)
    flag.StringVar(&argPort, "port", "", "port, required.")
    flag.Parse()

	ProxyServer("0.0.0.0", argPort)
}

func ProxyServer(ip string, port string) {
    s := NewServer()
    s.Initialize()
    ln, err := tls.Listen("tcp", fmt.Sprintf(":%s", port), s.Config)

	// ln, err := net.Listen("tcp", fmt.Sprintf("%s:%d", ip, port))
	if err != nil {
		fmt.Println(err)
		return
	}
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println(err)
			continue
		}
		go handleConnection(conn.(*tls.Conn))
	}
}

func processConn(conn *net.TCPConn) *net.TCPConn {
	var t time.Time
	t.Add(10000000)
	conn.SetDeadline(t)
	conn.SetReadDeadline(t)

	conn.SetReadBuffer(8000)
	conn.SetWriteBuffer(8000)

	return conn
}

type Query []byte

func (self *Query) Host() string {
	query := ([]byte)(*self)
	start := bytes.Index(query, []byte("Host:"))
	if start < 0 {
		return ""
	}
	end := bytes.IndexAny(query[start:], "\r\n")
	if end < 0 {
		return ""
	}
	end += start

	hostline := string(query[start:end])
	host := strings.TrimPrefix(hostline, "Host:")
	host = strings.Trim(host, "\r\n ")
	if strings.Index(host, ":") < 0 {
		host += ":80"
	}
	return host
}

func (self *Query) HostPort() string {
	query := ([]byte)(*self)
	if bytes.Index(query, []byte(":")) > 0 {
		return string(query)
	}
	return fmt.Sprintf("%s:80", query)
}

func find_n(self []byte, sep byte, n int) int {
	for i, c := range self {
		if c == sep {
			n--
			if n == 0 {
				return i
			}
		}
	}
	return -1
}
func (self *Query) URL() string {
	query := ([]byte)(*self)
	i := bytes.IndexAny(query, "\r\n")
	return string(query[:i])
}

func (self *Query) URL_cmd() string {
	url := self.URL()
	i := strings.Index(url, " ")
	return url[:i]
}

func (self *Query) URL_url() string {
	url := self.URL()
	return strings.Split(url, " ")[1]
}

func (self *Query) Fix() []byte {
	query := ([]byte)(*self)
	url := self.URL_url()
	if strings.Index(url, "://") > 0 {
		i := strings.Index(url, "://")
		j := strings.Index(url[i+3:], "/")
		tmp_url := url[i+3+j:]
		content := bytes.Replace(query, []byte(url), []byte(tmp_url), 1)
		// content = bytes.Replace(content, []byte("Connection: keep-alive"), []byte("Connection: Close"), 1)
		// content = bytes.Replace(content, []byte("Connection: Keep-Alive"), []byte("Connection: Close"), 1)
		return content
	}
	return query
}

func handleConnection(conn *tls.Conn) {
	//processConn(conn)
	// fmt.Printf("[TRACE] Incoming: %s <-> %s\n", conn.LocalAddr().String(), conn.RemoteAddr().String())

	var recvbuff [8192]byte
	read, err := conn.Read(recvbuff[:])
	if err != nil {
		fmt.Errorf("[ERROR] %s\n", err.Error())
		conn.Close()
		return
	}

	if read == len(recvbuff) {
		fmt.Print("[ERROR] to long. not except.\n")
		conn.Close()
		return
	}

	switch string(recvbuff[:7]) {
	case "CONNECT":
		n := bytes.Index(recvbuff[:], []byte("\r"))
		url := string(recvbuff[:n])
		host := strings.Split(url, " ")[1]
		rconn, err := net.Dial("tcp", host)
		if err != nil {
			fmt.Printf("[ERROR] %s\n", err)
			conn.Close()
			return
		}

		fmt.Printf("[INFO] %s\n", url)
		conn.Write([]byte(ConnectResponse))
		Trans(conn, rconn.(*net.TCPConn))
		return
	default:
	}

	query := (Query)(recvbuff[:read])
	host := query.Host()
	if len(host) == 0 {
		fmt.Print("[ERROR] dont have Host\n")
		conn.Close()
		return
	}
	// fmt.Printf("[INFO] %s, %s\n", host, query.URL())

	rconn, err := net.Dial("tcp", host)
	if err != nil {
		fmt.Printf("[ERROR] %s\n", err)
		conn.Close()
		return
	}
	fmt.Printf("[INFO] %s[%s] %s\n", host, rconn.RemoteAddr().String(), query.URL())
	processConn(rconn.(*net.TCPConn))
	content := query.Fix()
	fmt.Printf("[TRACE] %d:%s\n", len(content), query.URL())

	// fmt.Printf("[TRACE] %d:%s\n", read, query.URL())
	// query = (Query)(content)
	// fmt.Printf("[TRACE] %d:%s\n", len(content), string(content))
	rconn.Write(content)
	Trans(conn, rconn.(*net.TCPConn))
	// lconn := SConn_new(conn)
	// Trans(lconn.TCPConn, rconn.(*net.TCPConn))
}

func Trans(client *tls.Conn, remote *net.TCPConn) {
	var pipe chan []byte = make(chan []byte, 1)
	var pipe1 chan []byte = make(chan []byte, 1)
	go ReadConntls(client, pipe)
	go WriteConntls(client, pipe1)
	go ReadConn(remote, pipe1)
	go WriteConn(remote, pipe)
}

func WriteConntls(conn *tls.Conn, pipe <-chan []byte) {
    defer conn.Close()
    // defer fmt.Printf("[INFO] WriteConn.Close(%s)\n", conn.RemoteAddr())

    for {
        buffer, ok := <-pipe
        if !ok {
            return
        }
        // fmt.Printf("[INFO] %s, %d <- \n", conn.RemoteAddr(), len(buffer))
        // fmt.Printf("[INFO] %s \n", string(buffer[:32]))
        write, err := conn.Write(buffer)
        if err != nil {
            fmt.Printf("[ERROR] Write(%s): %s\n", conn.RemoteAddr(), err)
            break
        }
        if write != len(buffer) {
            fmt.Printf("[ERROR] dont Write full.\n")
        }
    }
}


func ReadConntls(conn *tls.Conn, pipe chan<- []byte) {
    defer func() {
        conn.Close()
        close(pipe)
    }()
    // defer fmt.Printf("[INFO] ReadConn.Close(%s, %s)\n", conn.RemoteAddr(), pipe)

    for {
        buffer := make([]byte, 8192)
        reat, err := conn.Read(buffer)
        if err != nil {
            // fmt.Printf("[ERROR] Read.err = %s. %s\n", err, conn.RemoteAddr())
            return
        }
        if reat == 0 {
            // fmt.Printf("[INFO] %s closed.", conn.RemoteAddr())
            return
        }
        switch string(buffer[:4]) {
        case "GET ", "CONN", "POST", "OPTI", "HEAD", "DELE", "PUT ", "TRAC":
            query := (Query)(buffer[:reat])
            content := query.Fix()
            fmt.Printf("[INFO] %s[%s]:%s\n", query.Host(), conn.RemoteAddr(), query.URL())
            // fmt.Printf("[INFO] %s, <- %d/%d\n", conn.RemoteAddr(), len(content), reat);
            pipe <- content
            continue
        case "HTTP":
            n := bytes.Index(buffer[:reat], []byte("\r\n"))
            fmt.Printf("[TRACE] %s:%s\n", conn.RemoteAddr(), string(buffer[:n]))
            // fmt.Printf("[TRACE] %d,%s", reat, string(buffer[:reat]));
        default:
            // fmt.Printf("[TRACE] something...\n")
        }
        // fmt.Printf("[INFO] %s, <- %d\n", conn.RemoteAddr(), reat)
        pipe <- buffer[0:reat]
    }
}

func ReadConn(conn *net.TCPConn, pipe chan<- []byte) {
	defer func() {
		conn.Close()
		close(pipe)
	}()
	// defer fmt.Printf("[INFO] ReadConn.Close(%s, %s)\n", conn.RemoteAddr(), pipe)

	for {
		buffer := make([]byte, 8192)
		reat, err := conn.Read(buffer)
		if err != nil {
			// fmt.Printf("[ERROR] Read.err = %s. %s\n", err, conn.RemoteAddr())
			return
		}
		if reat == 0 {
			// fmt.Printf("[INFO] %s closed.", conn.RemoteAddr())
			return
		}
		switch string(buffer[:4]) {
		case "GET ", "CONN", "POST", "OPTI", "HEAD", "DELE", "PUT ", "TRAC":
			query := (Query)(buffer[:reat])
			content := query.Fix()
			fmt.Printf("[INFO] %s[%s]:%s\n", query.Host(), conn.RemoteAddr(), query.URL())
			// fmt.Printf("[INFO] %s, <- %d/%d\n", conn.RemoteAddr(), len(content), reat);
			pipe <- content
			continue
		case "HTTP":
			n := bytes.Index(buffer[:reat], []byte("\r\n"))
			fmt.Printf("[TRACE] %s:%s\n", conn.RemoteAddr(), string(buffer[:n]))
			// fmt.Printf("[TRACE] %d,%s", reat, string(buffer[:reat]));
		default:
			// fmt.Printf("[TRACE] something...\n")
		}
		// fmt.Printf("[INFO] %s, <- %d\n", conn.RemoteAddr(), reat)
		pipe <- buffer[0:reat]
	}
}
func WriteConn(conn *net.TCPConn, pipe <-chan []byte) {
	defer conn.Close()
	// defer fmt.Printf("[INFO] WriteConn.Close(%s)\n", conn.RemoteAddr())

	for {
		buffer, ok := <-pipe
		if !ok {
			return
		}
		// fmt.Printf("[INFO] %s, %d <- \n", conn.RemoteAddr(), len(buffer))
		// fmt.Printf("[INFO] %s \n", string(buffer[:32]))
		write, err := conn.Write(buffer)
		if err != nil {
			fmt.Printf("[ERROR] Write(%s): %s\n", conn.RemoteAddr(), err)
			break
		}
		if write != len(buffer) {
			fmt.Printf("[ERROR] dont Write full.\n")
		}
	}
}

func nslookup(host string) error {
	names, err := net.LookupAddr(host)

	for i, s := range names {
		fmt.Printf(" %d %s \n", i, s)
	}

	return err
}
