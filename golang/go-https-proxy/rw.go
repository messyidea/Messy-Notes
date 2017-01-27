// +build go1.6

package main

import (
	"io"
	"net/http"
	"time"
)

type readDeadline interface {
	SetReadDeadline(t time.Time) error
}

type writeDeadline interface {
	SetWriteDeadline(t time.Time) error
}

type timeoutReader struct {
	r       io.Reader
	timeout time.Duration
}

func (tr timeoutReader) Read(p []byte) (n int, err error) {
	if sd, ok := tr.r.(readDeadline); ok {
		sd.SetReadDeadline(time.Now().Add(tr.timeout))
	}
	n, err = tr.r.Read(p)
	return
}

type timeoutWriter struct {
	w       io.Writer
	timeout time.Duration
}

func (tw timeoutWriter) Write(p []byte) (n int, err error) {
	if sd, ok := tw.w.(writeDeadline); ok {
		sd.SetWriteDeadline(time.Now().Add(tw.timeout))
	}
	n, err = tw.w.Write(p)
	return
}

type flushWriter struct {
	w io.Writer
}

func (fw flushWriter) Write(p []byte) (n int, err error) {
	n, err = fw.w.Write(p)
	if f, ok := fw.w.(http.Flusher); ok {
		f.Flush()
	}
	return
}
