// https-proxy-to-http-proxy project main.go
package main

import (
	"crypto/tls"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"time"
)

var (
	listenAddr string
	remoteAddr string
	debug      bool
)

type tlsClientCon struct {
	conn    *tls.Conn
	rawConn net.Conn
}

func (c *tlsClientCon) Write(b []byte) (int, error) {
	return c.conn.Write(b)
}

func (c *tlsClientCon) Read(b []byte) (int, error) {
	return c.conn.Read(b)
}

func (c *tlsClientCon) CloseWrite() error {
	if cwc, ok := c.rawConn.(interface {
		CloseWrite() error
	}); ok {
		return cwc.CloseWrite()
	}
	return nil
}

func (c *tlsClientCon) CloseRead() error {
	if cwc, ok := c.rawConn.(interface {
		CloseRead() error
	}); ok {
		return cwc.CloseRead()
	}
	return nil
}

func dialTLS(network, addr string) (*tlsClientCon, error) {
	rawConn, err := net.Dial(network, addr)
	if err != nil {
		return nil, err
	}

	conn := tls.Client(rawConn, &tls.Config{
		InsecureSkipVerify: true,
	})

	return &tlsClientCon{conn, rawConn}, nil
}

func main() {
	flag.StringVar(&listenAddr, "listen", ":8888", "listen address")
	flag.StringVar(&remoteAddr, "remote", "", "remote address")
	flag.BoolVar(&debug, "debug", false, "Debug")

	flag.Parse()

	if remoteAddr == "" {
		fmt.Println("remoteAddr is needed!")
		os.Exit(1)
	}
	startServe(listenAddr, remoteAddr)
	<-make(chan bool)
}

func startServe(listenAddr, remoteAddr string) {
	ln, err := net.Listen("tcp", listenAddr)
	if err != nil {
		log.Fatalln(err)
		return
	}
	fmt.Printf("Start serving on %s\n", ln.Addr().String())
	go func() {
		for {
			localConn, err := ln.Accept()
			if err != nil {
				log.Println(err)
				time.Sleep(time.Millisecond)
				continue
			}
			go func() {
				remoteConn, err := dialTLS("tcp", remoteAddr)
				if err != nil {
					log.Println("Error while connect to remote: ", err)
					localConn.Close()
					return
				}
				go func() {
					var err error
					var n int
					buf := make([]byte, 2048)
					for {
						n, err = localConn.Read(buf)
						if err != nil {
							break
						}
						_, err = remoteConn.Write(buf[:n])
						if err != nil {
							break
						}
					}
					if err != io.EOF {
						log.Println(err)
					}
					if conn, ok := localConn.(*net.TCPConn); ok {
						conn.CloseRead()
					}
					remoteConn.CloseWrite()
				}()
				go func() {
					var err error
					var n int
					buf := make([]byte, 2048)
					for {
						n, err = remoteConn.Read(buf)
						if err != nil {
							break
						}
						_, err = localConn.Write(buf[:n])
						if err != nil {
							break
						}
					}
					if err != io.EOF {
						log.Println(err)
					}

					if conn, ok := localConn.(*net.TCPConn); ok {
						conn.CloseWrite()
					} else {
						localConn.Close()
					}
					remoteConn.CloseWrite()
				}()
			}()

		}
	}()
}
