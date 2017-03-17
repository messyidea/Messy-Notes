package main

import (
	"encoding/base64"
	"net/http"
)

func BasicAuth(user, pass string) string {
	return "Basic " + base64.StdEncoding.EncodeToString(
		[]byte(user+":"+pass))
}

func CopyHeader(dst, src http.Header) {
	for key,values := range src {
		for _,value := range values {
			dst.Add(key,value)
		}
	}
}
