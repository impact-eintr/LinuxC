package main

import (
	"fmt"
)

func putNum(num int, intchan chan int) {
	for i := 2; i < num; i++ {
		intchan <- i

	}
	close(intchan)

}
func getPrime(intchan, primechan chan int, exitchan chan bool) {
	var flag bool

	for {
		num, ok := <-intchan
		flag = true
		if !ok {
			break

		}
		for i := 2; i < num; i++ {
			if num%i == 0 {
				flag = false
				break
				//primechan <- num

			}

		}
		if flag {
			primechan <- num

		}

	}
	exitchan <- true
	//close(primechan)
	fmt.Println("管道中无数据")

}

func main() {
	num := 5000000000
	intchan := make(chan int, num)
	primechan := make(chan int, 20)
	exitchan := make(chan bool, 12)
	putNum(num, intchan)
	for i := 0; i < 12; i++ {
		go getPrime(intchan, primechan, exitchan)

	}
	go func() {
		for i := 0; i < 4; i++ {
			<-exitchan

		}
		close(primechan)

	}()

	for {
		res, ok := <-primechan
		if !ok {
			break

		}
		fmt.Println(res)

	}

}
