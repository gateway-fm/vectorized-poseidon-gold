#ifndef POSEIDONGOLD
#define POSEIDONGOLD

#include <immintrin.h>
#include <stdio.h>

#include "poseidongold_element_inc.h"
#include "poseidongold_hash_scalar.h"
#include "poseidongold_hash_utils.h"

#ifdef __AVX2__
#include "poseidongold_hash_avx2.h"
#endif

#ifdef __AVX512F__
#include "poseidongold_hash_avx512.h"
#endif

#ifdef __AVX2__
extern bool PoseidongoldHash_initializedAvx2;
#endif

#ifdef __AVX512F__
extern bool PoseidongoldHash_initializedAvx512;
#endif

void PoseidongoldHash_result_scalar(PoseidongoldElement *, const PoseidongoldElement *);

#ifdef __AVX2__
void PoseidongoldHash_result_avx2(PoseidongoldElement *, const PoseidongoldElement *);
#endif

#ifdef __AVX512F__
void PoseidongoldHash_result_avx512(PoseidongoldElement *, const PoseidongoldElement *);
#endif

#endif
