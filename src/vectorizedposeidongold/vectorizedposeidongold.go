package vectorizedposeidongold

/*
#cgo CFLAGS: -fPIC
#cgo CXXFLAGS: -fPIC
#cgo LDFLAGS: -Wl,-rpath,$ORIGIN:${SRCDIR}/../../build -fPIC -lvectorizedposeidongold.x86_64 -lvectorizedposeidongold.x86_64.avx2 -lvectorizedposeidongold.x86_64.avx512 -lgmp -lomp -L${SRCDIR}/../../build
#include "../../build/vectorizedposeidongold.h"
*/
import "C"

import (
	"unsafe"

	"golang.org/x/sys/cpu"
)

func Hash(in [8]uint64, capacity [4]uint64) ([4]uint64, error) {
	var result [4]uint64 = [4]uint64{0, 0, 0, 0}
	HashWithResult(&in, &capacity, &result)
	return result, nil
}

func HashDirect(in *[8]uint64, capacity *[4]uint64) [4]uint64 {
	var result [4]uint64 = [4]uint64{0, 0, 0, 0}
	HashWithResult(in, capacity, &result)
	return result
}

func HashWithResult(in *[8]uint64, capacity *[4]uint64, result *[4]uint64) {
	cInput := (*C.ulonglong)(unsafe.Pointer(&in[0]))
	cCapacity := (*C.ulonglong)(unsafe.Pointer(&capacity[0]))
	cResult := (*C.ulonglong)(unsafe.Pointer(&result[0]))

	if cpu.X86.HasAVX512F {
		C.hashAvx512(cInput, cCapacity, cResult)
	} else if cpu.X86.HasAVX2 {
		C.hashAvx2(cInput, cCapacity, cResult)
	} else {
		C.hash(cInput, cCapacity, cResult)
	}
}
