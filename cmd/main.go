package main

import "C"

import (
	"fmt"
	"math/rand"
	"time"

	"golang.org/x/sys/cpu"

	"github.com/gateway-fm/vectorized-poseidon-gold/src/vectorizedposeidongold"
)

func main() {
	if cpu.X86.HasAVX512F {
		fmt.Println("Using AVX512")
	} else if cpu.X86.HasAVX2 {
		fmt.Println("Using AVX2")
	} else {
		fmt.Println("Using x64 instructions")
	}

	// var SIZE int = 1000000
	var SIZE int = 1000000
	var TESTS int = 4

	var input [][8]uint64 = make([][8]uint64, SIZE)
	var capacity [4]uint64 = [4]uint64{0, 0, 0, 0}
	var result [4]uint64

	for i := 0; i < SIZE; i++ {
		for j := 0; j < 8; j++ {
			input[i][j] = rand.Uint64()
		}
	}

	var average time.Duration

	for k := 0; k < TESTS; k++ {
		start := time.Now()
		fmt.Printf("Run %d of %d\n", k+1, TESTS)
		for i := 0; i < SIZE; i++ {
			// cInput := (*C.ulonglong)(unsafe.Pointer(&input[i][0]))
			// cCapacity := (*C.ulonglong)(unsafe.Pointer(&capacity[0]))
			// cResult := (*C.ulonglong)(unsafe.Pointer(&result[0]))
			// C.hash(cInput, cCapacity, cResult)
			vectorizedposeidongold.HashWithResult(&input[i], &capacity, &result)

			// result, _ = Hash(input[i], capacity)
			// for j := 0; j < 4; j++ {
			// 	if result[j] != resultTest[j] {
			// 		panic("DIFF")
			// 	}
			// }

			// if err != nil {
			// 	panic(err)
			// }
			// fmt.Printf("Desired hash of %s is: %s\n", fmt.Sprintf("%v-%v", input[i], capacity), fmt.Sprintf("%v", h))
		}
		elapsed := time.Since(start)
		average += elapsed
		fmt.Printf("%d milliseconds\n", elapsed.Milliseconds())
	}
	fmt.Printf("%d average milliseconds\n", average.Milliseconds()/int64(TESTS))

	fmt.Println(result)

	// h, err := Hash(in, capacity)
	// if err != nil {
	// 	panic(err)
	// }
	// fmt.Println("Desired hash of %s is: %s", fmt.Sprintf("%v-%v", in, capacity), fmt.Sprintf("%v", h))

	// out := C.CString("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
	// defer C.free(unsafe.Pointer(out))
	// C.fromString(out)
	// fmt.Printf("Id: %s\n", C.GoString(out))
}
