#ifndef POSEIDONGOLD_ELEMENT_AVX512
#define POSEIDONGOLD_ELEMENT_AVX512

#ifdef __AVX512__

#include <immintrin.h>

#include "poseidongold_element.h"

extern __m512i P8;
extern __m512i P8_n;
extern __m512i sqmask8;

void PoseidongoldElement_initializeAvx512();

void PoseidongoldElement_load_avx512(__m512i *, const PoseidongoldElement *);

void PoseidongoldElement_store_avx512(PoseidongoldElement *, const __m512i *);

void PoseidongoldElement_add_avx512(__m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_add_avx512_b_c(__m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_sub_avx512_b_c(__m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_square_avx512(__m512i *, __m512i *);

void PoseidongoldElement_square_avx512_128(__m512i *, __m512i *, const __m512i *);

void PoseidongoldElement_mult_avx512(__m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_mult_avx512_72(__m512i *, __m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_mult_avx512_128(__m512i *, __m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_mmult_avx512(__m512i *, __m512i *, __m512i *, const PoseidongoldElement M[144]);

void PoseidongoldElement_mmult_avx512_8(__m512i *, __m512i *, __m512i *, const PoseidongoldElement M_8[144]);

void PoseidongoldElement_mmult_avx512_4x12(__m512i *, const __m512i *, const __m512i *, const __m512i *, const PoseidongoldElement M[48]);

void PoseidongoldElement_mmult_avx512_4x12_8(__m512i *, const __m512i *, const __m512i *, const __m512i *, const PoseidongoldElement M[48]);

void PoseidongoldElement_spmv_avx512_4x12(__m512i *, const __m512i *, const __m512i *, const __m512i *, const PoseidongoldElement b[12]);

void PoseidongoldElement_spmv_avx512_4x12_8(__m512i *, const __m512i *, const __m512i *, const __m512i *, const PoseidongoldElement b[12]);

void PoseidongoldElement_reduce_avx512_96_64(__m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_reduce_avx512_128_64(__m512i *, const __m512i *, const __m512i *);

void PoseidongoldElement_toCanonical_avx512(__m512i *, const __m512i *);

#endif  // AVX512 guard

#endif
