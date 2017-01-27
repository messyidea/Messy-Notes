package main

import (
	"encoding/base64"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"time"
)

const SoTimeout = 30 * time.Second

var (
	argListen         string
	argUserPass       string
	argKey            string
	argCert           string
	basicAuthUserPass string
)

func main() {
	flag.StringVar(&argListen, "listen", "", "Proxy listen, required.")
	flag.StringVar(&argUserPass, "userpass", "", "user:pass")
	flag.StringVar(&argKey, "key", "private.key", "your key (default private.key)")
	flag.StringVar(&argCert, "cert", "public.crt", "your cert (default public.cert)")
	flag.Parse()

	if argListen == "" {
		log.Fatalln("-listen argument required.")
	}

	if argUserPass != "" {
		basicAuthUserPass = "Basic " + base64.StdEncoding.EncodeToString(
			[]byte(argUserPass))
	}

	log.Fatalln(http.ListenAndServeTLS(argListen, argCert, argKey, new(server)))
}

type server struct{}

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
	} else if r.Host == "" {
		http.NotFound(w, r)
	} else {
		handleHttpProxy(w, r)
	}
}

func handleHttpProxy(w http.ResponseWriter, r *http.Request) {

	if r.URL.Host == "" {
		r.URL.Host = r.Host
	}

	if r.ContentLength == 0 {
		io.Copy(ioutil.Discard, r.Body)
		r.Body.Close()
		r.Body = nil
	}

	if r.URL.Scheme == "" {
		r.URL.Scheme = "http"
	}

	if r.ProtoMajor == 2 && r.ProtoMinor == 0 {
		r.ProtoMajor = 1
		r.ProtoMinor = 1
		r.Proto = "HTTP/1.1"
	}

	resp, err := DefaultTransport.RoundTrip(r)
	if err != nil {
		log.Println(r.Method, r.URL, err)
		w.WriteHeader(http.StatusBadGateway)
		fmt.Fprintln(w, err.Error())
	} else {
		fmt.Println(r.Method, r.URL, resp.Status)
		CopyHeader(w.Header(), resp.Header)
		w.WriteHeader(resp.StatusCode)
		io.Copy(w, resp.Body)
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

	oconn, err := DefaultTunnelingDial("tcp", r.RequestURI)
	if oconn != nil {
		defer oconn.Close()
	}
	if err != nil {
		return
	}

	w.WriteHeader(http.StatusOK)

	var h2 bool = r.ProtoMajor == 2 && r.ProtoMinor == 0

	if h2 {
		f, _ := w.(http.Flusher)
		f.Flush()
		fmt.Println(r.Method, r.URL, "OK")
		closing := make(chan bool, 2)
		go func() {
			io.Copy(timeoutWriter{oconn, SoTimeout}, timeoutReader{r.Body, SoTimeout})
			closing <- true
		}()
		go func() {
			io.Copy(timeoutWriter{flushWriter{w}, SoTimeout}, timeoutReader{oconn, SoTimeout})
			closing <- true
		}()
		<-closing
	} else {
		iconn, _, err := w.(http.Hijacker).Hijack()
		if iconn != nil {
			defer iconn.Close()
		}
		if err != nil {
			return
		}
		iconn = &TimeoutConn{
			Inner:   iconn,
			Timeout: 2 * time.Minute,
		}

		fmt.Println(r.Method, r.URL, "OK")
		closing := make(chan bool, 2)
		go func() {
			io.Copy(iconn, oconn)
			closing <- true
		}()
		go func() {
			io.Copy(oconn, iconn)
			closing <- true
		}()
		<-closing
	}

}
