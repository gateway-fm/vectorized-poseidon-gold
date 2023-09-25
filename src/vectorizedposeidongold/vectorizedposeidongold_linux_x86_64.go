//go:build linux && amd64

package vectorizedposeidongold

/*
#cgo CFLAGS: -fPIC
#cgo CXXFLAGS: -fPIC
#cgo LDFLAGS: -Wl,-rpath,$ORIGIN/linux:${SRCDIR}/../../build/linux -fPIC -lvectorizedposeidongold.linux.x86_64 -lvectorizedposeidongold.linux.x86_64.avx2 -lvectorizedposeidongold.linux.x86_64.avx512 -lgmp -lomp -L${SRCDIR}/../../build/linux
#include "../../build/vectorizedposeidongold.h"
*/
import "C"

import (
	"unsafe"

	"golang.org/x/sys/cpu"
)

func HashWithResult(in *[8]uint64, capacity *[4]uint64, result *[4]uint64) {
	cInput := (*C.ulonglong)(unsafe.Pointer(&in[0]))
	cCapacity := (*C.ulonglong)(unsafe.Pointer(&capacity[0]))
	cResult := (*C.ulonglong)(unsafe.Pointer(&result[0]))

	// if runtime.GOOS == "linux" && runtime.GOARCH == "amd64" {
	if cpu.X86.HasAVX512F {
		C.hashAvx512(cInput, cCapacity, cResult)
	} else if cpu.X86.HasAVX2 {
		C.hashAvx2(cInput, cCapacity, cResult)
	} else {
		C.hash(cInput, cCapacity, cResult)
	}
	// } else {
	// res, _ := goldenposeidon.Hash(*in, *capacity)
	// result[0] = res[0]
	// result[1] = res[1]
	// result[2] = res[2]
	// result[3] = res[3]
	// }
}
