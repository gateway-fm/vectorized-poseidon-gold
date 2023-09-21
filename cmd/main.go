package main

import "C"

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"

	"golang.org/x/sys/cpu"

	"github.com/gateway-fm/vectorized-poseidon-gold/src/vectorizedposeidongold"
	goldenposeidon "github.com/iden3/go-iden3-crypto/goldenposeidon"
)

func main() {
	if cpu.X86.HasAVX512F {
		fmt.Println("Using AVX512")
	} else if cpu.X86.HasAVX2 {
		fmt.Println("Using AVX2")
	} else {
		fmt.Println("Using x64 instructions")
	}
	if runtime.GOOS == "linux" && runtime.GOARCH == "amd64" {
		fmt.Println("Using vectorized version")
	} else {
		fmt.Println("Using iden3 version")
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

	var averageVectorized time.Duration

	for k := 0; k < TESTS; k++ {
		start := time.Now()
		fmt.Printf("Run %d of %d\n", k+1, TESTS)
		for i := 0; i < SIZE; i++ {
			vectorizedposeidongold.HashWithResult(&input[i], &capacity, &result)
		}
		elapsed := time.Since(start)
		averageVectorized += elapsed
		fmt.Printf("%d milliseconds\n", elapsed.Milliseconds())
	}
	fmt.Printf("%d average vectorized milliseconds\n", averageVectorized.Milliseconds()/int64(TESTS))
	fmt.Println(result)

	var averageRegular time.Duration
	for k := 0; k < TESTS; k++ {
		start := time.Now()
		fmt.Printf("Run %d of %d\n", k+1, TESTS)
		for i := 0; i < SIZE; i++ {
			result, _ = goldenposeidon.Hash(input[i], capacity)
		}
		elapsed := time.Since(start)
		averageRegular += elapsed
		fmt.Printf("%d milliseconds\n", elapsed.Milliseconds())
	}
	fmt.Printf("%d average regular milliseconds\n", averageRegular.Milliseconds()/int64(TESTS))
	fmt.Println(result)

	fmt.Printf("Performance gain %.2f\n", float64(averageRegular.Milliseconds())/float64(averageVectorized.Milliseconds()))

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
