package vectorizedposeidongold_test

import (
	"fmt"
	"math/rand"
	"testing"

	"github.com/gateway-fm/vectorized-poseidon-gold/src/vectorizedposeidongold"

	goldenposeidon "github.com/iden3/go-iden3-crypto/goldenposeidon"
)

func TestHashWithResult(t *testing.T) {
	var SIZE int = 8

	var input [][8]uint64 = make([][8]uint64, SIZE)
	var capacity [4]uint64 = [4]uint64{0, 0, 0, 0}
	var result [4]uint64
	var passedTest bool

	rand.Seed(1)
	for i := 0; i < SIZE; i++ {
		for j := 0; j < 8; j++ {
			input[i][j] = rand.Uint64()
		}
		fmt.Printf("{%dULL,%dULL,%dULL,%dULL,%dULL,%dULL,%dULL,%dULL},\n", input[i][0], input[i][1], input[i][2], input[i][3], input[i][4], input[i][5], input[i][6], input[i][7])
	}

	for i := 0; i < SIZE; i++ {
		iden3HashResult, _ := goldenposeidon.Hash(input[i], capacity)
		vectorizedposeidongold.HashWithResult(&input[i], &capacity, &result)

		passedTest = true
		for j := 0; j < 4; j++ {
			if result[j] != iden3HashResult[j] {
				passedTest = false
			}
		}

		if !passedTest {
			t.Errorf("Expected Hash %v, Actual Hash %v", iden3HashResult, result)
			break
		} else {
			fmt.Printf("{%dULL, %dULL, %dULL, %dULL},\n", result[0], result[1], result[2], result[3])
		}
	}
}
