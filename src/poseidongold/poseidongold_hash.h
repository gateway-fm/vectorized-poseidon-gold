#ifndef POSEIDONGOLD
#define POSEIDONGOLD

#include <immintrin.h>

#include "poseidongold_element_inc.h"
#include "poseidongold_hash_scalar.h"
#include "poseidongold_hash_utils.h"
#ifdef __AVX2__
#include "poseidongold_hash_avx2.h"
#endif

#ifdef __AVX2__
extern bool PoseidongoldHash_initializedAvx2;
#endif

void PoseidongoldHash_result_scalar(PoseidongoldElement *, const PoseidongoldElement *);
#ifdef __AVX2__
void PoseidongoldHash_result_avx2(PoseidongoldElement *, const PoseidongoldElement *);
#endif

#endif
