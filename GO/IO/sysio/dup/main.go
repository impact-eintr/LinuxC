package main

import (
	"fmt"
	"os"
)

func main() {
	file, _ := os.Open("./log")
	os.Stdout.Close()
	fd := file.Fd()

	fmt.Println(fd)
}
