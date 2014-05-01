package main

import (
    "net/http"
)

const (
      msg = "HTTP/1.0 200 OK\r\nContent-Length: 5\r\n\r\nPong!\r\n"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte(msg));
	})
       http.ListenAndServe(":8000", nil)
}