package main

import (
    "fmt"
    "net/http"
    "flag"
)

var (
    argPort string
)

func main() {
    flag.StringVar(&argPort, "port", "", "Port, default is 8080.")
    flag.Parse()

    if argPort == "" {
        argPort = "8080"
    }


    fs := http.FileServer(http.Dir("."))
    http.Handle("/", fs)
    bind :=fmt.Sprintf("%s:%s", "0.0.0.0", argPort)
    fmt.Printf("listening on %s...", bind)
    http.ListenAndServe(bind, nil)

}

