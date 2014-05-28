package main

import (
    "net/http"
    "log"
)

const (
      msg = "Pong!\r\n"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
	        go func() {
		  w.Header().Set("Content-Type", "text/plain")
	        	w.Write([]byte(msg))
			}()
	})
       log.Fatal(http.ListenAndServe(":8000", nil))
}