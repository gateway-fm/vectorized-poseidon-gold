//go:build !linux || !amd64

package vectorizedposeidongold

import (
	goldenposeidon "github.com/iden3/go-iden3-crypto/goldenposeidon"
)

func HashWithResult(in *[8]uint64, capacity *[4]uint64, result *[4]uint64) {
	res, _ := goldenposeidon.Hash(*in, *capacity)
	result[0] = res[0]
	result[1] = res[1]
	result[2] = res[2]
	result[3] = res[3]
}
