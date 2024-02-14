package vectorizedposeidongold

/*
#cgo CFLAGS: -fPIC -O2 -march=native -std=c17
#cgo LDFLAGS: -fPIC
#include "../poseidongold/poseidongold_element_avx2.h"
#include "../poseidongold/poseidongold_element_avx2.c"
#include "../poseidongold/poseidongold_element_inc.h"
#include "../poseidongold/poseidongold_element_scalar.h"
#include "../poseidongold/poseidongold_element_scalar.c"
#include "../poseidongold/poseidongold_element_utils.h"
#include "../poseidongold/poseidongold_element_utils.c"
#include "../poseidongold/poseidongold_element.h"
#include "../poseidongold/poseidongold_element.c"
#include "../poseidongold/poseidongold_hash_avx2.h"
#include "../poseidongold/poseidongold_hash_avx2.c"
#include "../poseidongold/poseidongold_hash_scalar.h"
#include "../poseidongold/poseidongold_hash_scalar.c"
#include "../poseidongold/poseidongold_hash_utils.h"
#include "../poseidongold/poseidongold_hash_utils.c"
#include "../poseidongold/poseidongold_hash.h"
#include "../poseidongold/poseidongold_hash.c"
#include "../poseidongold/poseidongold_gointerface.h"
#include "../poseidongold/poseidongold_gointerface.c"
*/
import "C"

import (
	"unsafe"
)

func HashWithResult(in *[8]uint64, capacity *[4]uint64, result *[4]uint64) {
	cInput := (*C.ulonglong)(unsafe.Pointer(&in[0]))
	cCapacity := (*C.ulonglong)(unsafe.Pointer(&capacity[0]))
	cResult := (*C.ulonglong)(unsafe.Pointer(&result[0]))

	C.poseidongoldHash(cInput, cCapacity, cResult)
}

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
