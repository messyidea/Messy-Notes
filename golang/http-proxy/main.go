package main

import (
	"encoding/base64"
	"fmt"
	"flag"
	"io"
	"log"
	"net/http"
	"time"
)

var (
	argListen string
	argUserPass string
	basicAuthUserPass string
)

func main() {
	flag.StringVar(&argListen, "listen", "", "Proxy listen, required.")
	flag.StringVar(&argUserPass, "userpass", "", "user:pass")
	flag.Parse()

	if argListen == "" {
		log.Fatalln("-listen argument required.")
	}

	if argUserPass != "" {
		basicAuthUserPass = "Basic " + base64.StdEncoding.EncodeToString(
			[]byte(argUserPass))
	}

	log.Fatalln(http.ListenAndServe(argListen, new(server)))
}

type server struct {}

func (s *server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	if basicAuthUserPass != "" {
		auth := r.Header.Get("Proxy-Authorization")
		r.Header.Del("Proxy-Authorization")
		if auth != basicAuthUserPass {
            w.Header().Set("Proxy-Authenticate", "Basic realm=\"go-proxy\"")
			http.Error(w, "ProxyAuthRequired", http.StatusProxyAuthRequired)
			return
		}
	}

	if r.Method == "CONNECT" {
		handleTunneling(w, r)
	} else if r.URL.Host == "" {
		http.NotFound(w,r)
	} else {
		handleHttpProxy(w, r)
	}
}

func handleHttpProxy(w http.ResponseWriter, r *http.Request) {
	resp,err := DefaultTransport.RoundTrip(r)
	if err != nil {
		log.Println(r.Method, r.URL, err)
		w.WriteHeader(http.StatusBadGateway)
		fmt.Fprintln(w, err.Error())
	} else {
		fmt.Println(r.Method, r.URL, resp.Status)
		CopyHeader(w.Header(), resp.Header)
		w.WriteHeader(resp.StatusCode)
		io.Copy(w,resp.Body)
	}
}

func handleTunneling(w http.ResponseWriter, r *http.Request) {
	var err error
	defer func() {
		if err != nil {
			log.Println(r.Method, r.RequestURI, err)
			http.Error(w, err.Error(), http.StatusBadGateway)
		}
	}()

	oconn,err := DefaultTunnelingDial("tcp", r.RequestURI)
	if oconn != nil { defer oconn.Close() }
	if err != nil { return }

	// w.WriteHeader(http.StatusOK)

	iconn, bufrw, err := w.(http.Hijacker).Hijack()
	if iconn != nil { defer iconn.Close() }
	if err != nil { return }

	bufrw.WriteString("HTTP/1.0 200 Connection established\r\n")
	bufrw.WriteString("Proxy-agent: Netscape-Proxy/1.1\r\n")
	bufrw.WriteString("\r\n")
	bufrw.Flush()

	iconn = &TimeoutConn{
		Inner : iconn,
		Timeout : 2 * time.Minute,
	}

	fmt.Println(r.Method, r.URL, "OK")
	closing := make(chan bool, 2)
	go func(){
		io.Copy(iconn, oconn)
		closing<-true
	}()
	go func(){
		io.Copy(oconn, iconn)
		closing<-true
	}()
	<-closing
}
