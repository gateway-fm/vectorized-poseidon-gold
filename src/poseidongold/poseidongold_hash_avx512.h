#ifndef POSEIDONGOLD_AVX512
#define POSEIDONGOLD_AVX512

#ifdef __AVX512F__

#include <immintrin.h>

#include "poseidongold_element_inc.h"
#include "poseidongold_hash_utils.h"

void PoseidongoldHash_pow7_avx512(__m512i *, __m512i *, __m512i *);
void PoseidongoldHash_add_avx512(__m512i *, __m512i *, __m512i *, const PoseidongoldElement C_[SPONGE_WIDTH]);
void PoseidongoldHash_add_avx512_small(__m512i *, __m512i *, __m512i *, const PoseidongoldElement C_small[SPONGE_WIDTH]);
void PoseidongoldHash_dot_avx512(PoseidongoldElement c[2], const __m512i *a0, const __m512i *a1, const __m512i *a2, const PoseidongoldElement b[12]);

#endif  // AVX512 guard

#endif
