package main

import (
	"bufio"
	"crypto/tls"
	"errors"
	"net"
	"net/http"
	"net/url"
	"time"
)

var (
	TunnelingTimeout = time.Second * 60
)

//发起代理中转连接
type TunnelingDialer struct {
	DialFunc func(network, host string) (net.Conn, error)
	Proxy *url.URL
}

func DialOnProxyTunnel(n, host string, proxy *url.URL) (net.Conn, error) {
	return (&TunnelingDialer{
		DialFunc : nil,
		Proxy : proxy,
	}).Dial(n, host)
}

func (d *TunnelingDialer) Dial(n, host string) (net.Conn, error) {
	if d.Proxy == nil {
		return d.dialDirect(n, host)
	} else {
		return d.dialOnProxy(n, host)
	}
}

func (d *TunnelingDialer) dialDirect(n string, host string) (net.Conn, error) {
	dial := d.DialFunc
	if dial == nil {
		dial = (&net.Dialer{}).Dial
	}
	return dial(n, host)
}

func (d *TunnelingDialer) dialOnProxy(n, host string) (
		net.Conn, error) {
	var conn net.Conn
	var err error

	switch d.Proxy.Scheme {
	case "http":
		conn, err = d.dialDirect(n, d.Proxy.Host)
	case "https":
		conn, err = tls.Dial(n, d.Proxy.Host, &tls.Config{
			InsecureSkipVerify: true})
	default :
		err = errors.New("Unknown proxy scheme: " + d.Proxy.Scheme)
	}
	if err != nil { return nil, err }

	err = func() error {
		req := &http.Request{
			Method: "CONNECT",
			URL: &url.URL{Opaque: host},
			Host: host,
			Header: make(http.Header),
		}
		if d.Proxy.User != nil {
			user := d.Proxy.User.Username()
			pass,_ := d.Proxy.User.Password()
			req.Header.Set("Proxy-Authorization",
				BasicAuth(user,pass))
		}
		conn.SetDeadline(time.Now().Add(TunnelingTimeout))
		err := req.Write(conn)
			if err != nil { return err }
		conn.SetDeadline(time.Now().Add(TunnelingTimeout))
		resp, err := http.ReadResponse(bufio.NewReader(conn), req)
			if err != nil { return err }
		if resp.StatusCode != http.StatusOK {
			return errors.New(resp.Status)
		}
		return nil
	} ()

	if err != nil {
		conn.Close()
		return nil, err
	}

	conn.SetDeadline(time.Time{}) //unset deadline

	return conn, nil
}

var ErrConnTimeout = errors.New("Connection i/o timeout")

type TimeoutConn struct {
	Inner net.Conn
	Timeout time.Duration
}

func (c *TimeoutConn) Read(b []byte) (n int, err error) {
	return c.doTimeout(c.Inner.Read, b)
}

func (c *TimeoutConn) Write(b []byte) (n int, err error) {
	return c.doTimeout(c.Inner.Write, b)
}

func (c *TimeoutConn) doTimeout(f func(b []byte)(int,error), b []byte)(
		int,error) {
	chanFinished := make(chan bool, 1)
	var to bool
	go func() {
		select {
		case <-chanFinished:
		case <-time.After(c.Timeout):
			to = true
			c.Close()
		}
	}()
	n,err := f(b)
	chanFinished <- true
	if to {
		return 0, ErrConnTimeout
	}
	return n, err
}

func (c *TimeoutConn) Close() error { return c.Inner.Close() }

func (c *TimeoutConn) LocalAddr() net.Addr { return c.Inner.LocalAddr() }

func (c *TimeoutConn) RemoteAddr() net.Addr { return c.Inner.RemoteAddr() }

func (c *TimeoutConn) SetDeadline(t time.Time) error {
	return c.Inner.SetDeadline(t)
}
func (c *TimeoutConn) SetReadDeadline(t time.Time) error {
	return c.Inner.SetReadDeadline(t)
}
func (c *TimeoutConn) SetWriteDeadline(t time.Time) error {
	return c.Inner.SetWriteDeadline(t)
}

//发起可超时连接，指定时间没有数据传输就会关闭连接，
//而无论是否使用Conn.SetDeadline
type TimeoutDialer struct {
	DialFunc func(network, host string) (net.Conn, error)
	Timeout time.Duration
}

func (d *TimeoutDialer) Dial(n, host string) (net.Conn, error) {
	conn, err := d.DialFunc(n, host)
	if conn != nil {
		conn = &TimeoutConn{
			Inner : conn,
			Timeout : d.Timeout,
		}
	}
	return conn, err
}

//Defaults

var DefaultDialer = &TimeoutDialer{
	DialFunc : (&net.Dialer{
		Timeout:   30 * time.Second,
		KeepAlive: 30 * time.Second,
	}).Dial,
	Timeout : 2 * time.Minute,
}

func DefaultDial(n, host string) (net.Conn, error) {
	return DefaultDialer.Dial(n, host)
}

var DefaultTransport http.RoundTripper = &http.Transport{
	Proxy: http.ProxyFromEnvironment,
	Dial: DefaultDial,
	TLSHandshakeTimeout: 10 * time.Second,
}

var DefaultTunnelingDialer = func()*TunnelingDialer{
	result := &TunnelingDialer{
		DialFunc : DefaultDial,
	}

	dumpReq,err := http.NewRequest("GET", "https://www.baidu.com", nil)
	if err != nil { return nil }

	proxy,err := http.ProxyFromEnvironment(dumpReq)
	if err != nil { return nil }

	result.Proxy = proxy

	return result
}()

func DefaultTunnelingDial(n, host string) (net.Conn, error) {
	return DefaultTunnelingDialer.Dial(n, host)
}
