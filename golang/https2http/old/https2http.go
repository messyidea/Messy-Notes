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
				remoteConn, err := tls.Dial("tcp", remoteAddr, &tls.Config{
					InsecureSkipVerify: true,
				})
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
					rto := time.Now().Add(10)
					remoteConn.SetWriteDeadline(rto)
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
					rto := time.Now().Add(10)
					remoteConn.SetReadDeadline(rto)
				}()
			}()

		}
	}()
}
