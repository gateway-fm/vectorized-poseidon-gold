#ifndef POSEIDONGOLD_AVX2
#define POSEIDONGOLD_AVX2

#ifdef __AVX2__

#include <immintrin.h>

#include "poseidongold_element_inc.h"
#include "poseidongold_hash_utils.h"

void PoseidongoldHash_pow7_avx(__m256i *st0, __m256i *st1, __m256i *st2);
void PoseidongoldHash_add_avx(__m256i *st0, __m256i *st1, __m256i *st2, const PoseidongoldElement C_[SPONGE_WIDTH]);
void PoseidongoldHash_add_avx_small(__m256i *st0, __m256i *st1, __m256i *st2, const PoseidongoldElement C_small[SPONGE_WIDTH]);
PoseidongoldElement PoseidongoldHash_dot_avx(const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement b[12]);

#endif  // AVX2 guard

#endif
