#ifndef POSEIDONGOLD_ELEMENT_AVX2
#define POSEIDONGOLD_ELEMENT_AVX2

#ifdef __AVX2__

#include <immintrin.h>

#include "poseidongold_element.h"

extern __m256i MSB;
extern __m256i P;
extern __m256i P_n;
extern __m256i P_s;
extern __m256i sqmask;

void PoseidongoldElement_initializeAvx2();

void PoseidongoldElement_load_avx(__m256i *a, const PoseidongoldElement *a4);

void PoseidongoldElement_store_avx(PoseidongoldElement *a4, const __m256i *a);

void PoseidongoldElement_shift_avx(__m256i *a_s, const __m256i *a);

void PoseidongoldElement_add_avx(__m256i *c, const __m256i *a, const __m256i *b);

void PoseidongoldElement_add_avx_a_sc(__m256i *c, const __m256i *a_sc, const __m256i *b);

void PoseidongoldElement_add_avx_b_small(__m256i *c, const __m256i *a, const __m256i *b_small);

void PoseidongoldElement_add_avx_s_b_small(__m256i *c_s, const __m256i *a_s, const __m256i *b_small);

void PoseidongoldElement_sub_avx_s_b_small(__m256i *c_s, const __m256i *a_s, const __m256i *b);

void PoseidongoldElement_square_avx(__m256i *c, __m256i *a);

void PoseidongoldElement_square_avx_128(__m256i *c_h, __m256i *c_l, const __m256i *a);

void PoseidongoldElement_mult_avx(__m256i *c, const __m256i *a, const __m256i *b);

void PoseidongoldElement_mult_avx_72(__m256i *c_h, __m256i *c_l, const __m256i *a, const __m256i *b);

void PoseidongoldElement_mult_avx_128(__m256i *c_h, __m256i *c_l, const __m256i *a, const __m256i *b);

void PoseidongoldElement_mmult_avx(__m256i *a0, __m256i *a1, __m256i *a2, const PoseidongoldElement M[144]);

void PoseidongoldElement_mmult_avx_8(__m256i *a0, __m256i *a1, __m256i *a2, const PoseidongoldElement M_8[144]);

void PoseidongoldElement_mmult_avx_4x12(__m256i *b, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement M[48]);

void PoseidongoldElement_mmult_avx_4x12_8(__m256i *b, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement M_8[48]);

void PoseidongoldElement_spmv_avx_4x12(__m256i *c, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement b[12]);

void PoseidongoldElement_spmv_avx_4x12_8(__m256i *c, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement b_8[12]);

void PoseidongoldElement_reduce_avx_96_64(__m256i *c, const __m256i *c_h, const __m256i *c_l);

void PoseidongoldElement_reduce_avx_128_64(__m256i *c, const __m256i *c_h, const __m256i *c_l);

void PoseidongoldElement_toCanonical_avx_s(__m256i *a_sc, const __m256i *a_s);

#endif  // AVX2 guard

#endif
