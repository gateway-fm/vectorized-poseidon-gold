#ifndef POSEIDONGOLD_ELEMENT_INC
#define POSEIDONGOLD_ELEMENT_INC

#include "poseidongold_element.h"
#include "poseidongold_element_scalar.h"
#include "poseidongold_element_utils.h"

#ifdef __AVX2__
#include "poseidongold_element_avx2.h"
#endif

#ifdef __AVX512F__
#include "poseidongold_element_avx512.h"
#endif

#endif
