#include "poseidongold_element_avx2.h"

#ifdef __AVX2__

__m256i MSB;
__m256i P;
__m256i P_n;
__m256i P_s;
__m256i sqmask;

void PoseidongoldElement_initializeAvx2() {
    MSB = _mm256_set_epi64x(MSB_, MSB_, MSB_, MSB_);
    P = _mm256_set_epi64x(GOLDILOCKS_PRIME, GOLDILOCKS_PRIME, GOLDILOCKS_PRIME, GOLDILOCKS_PRIME);
    P_n = _mm256_set_epi64x(GOLDILOCKS_PRIME_NEG, GOLDILOCKS_PRIME_NEG, GOLDILOCKS_PRIME_NEG, GOLDILOCKS_PRIME_NEG);
    P_s = _mm256_xor_si256(P, MSB);
    sqmask = _mm256_set_epi64x(0x1FFFFFFFF, 0x1FFFFFFFF, 0x1FFFFFFFF, 0x1FFFFFFFF);
}

inline void PoseidongoldElement_load_avx(__m256i *a, const PoseidongoldElement *a4) { *a = _mm256_loadu_si256((__m256i *)(a4)); }

inline void PoseidongoldElement_store_avx(PoseidongoldElement *a4, const __m256i *a) { _mm256_storeu_si256((__m256i *)a4, *a); }

inline void PoseidongoldElement_shift_avx(__m256i *a_s, const __m256i *a) { *a_s = _mm256_xor_si256(*a, MSB); }

inline void PoseidongoldElement_add_avx(__m256i *c, const __m256i *a, const __m256i *b) {
    __m256i a_s, a_sc;
    PoseidongoldElement_shift_avx(&a_s, a);
    PoseidongoldElement_toCanonical_avx_s(&a_sc, &a_s);
    PoseidongoldElement_add_avx_a_sc(c, &a_sc, b);
}

inline void PoseidongoldElement_add_avx_a_sc(__m256i *c, const __m256i *a_sc, const __m256i *b) {
    const __m256i c0_s = _mm256_add_epi64(*a_sc, *b);

    __m256i mask_ = _mm256_cmpgt_epi64(*a_sc, c0_s);
    __m256i corr_ = _mm256_and_si256(mask_, P_n);
    __m256i c_s = _mm256_add_epi64(c0_s, corr_);

    PoseidongoldElement_shift_avx(c, &c_s);
}

inline void PoseidongoldElement_add_avx_b_small(__m256i *c, const __m256i *a, const __m256i *b_small) {
    __m256i a_s;
    PoseidongoldElement_shift_avx(&a_s, a);
    const __m256i c0_s = _mm256_add_epi64(a_s, *b_small);
    const __m256i mask_ = _mm256_cmpgt_epi32(a_s, c0_s);
    const __m256i corr_ = _mm256_srli_epi64(mask_, 32);  // corr=P_n when a_s > c0_s
    const __m256i temp = _mm256_add_epi64(c0_s, corr_);
    PoseidongoldElement_shift_avx(c, &temp);
}

inline void PoseidongoldElement_add_avx_s_b_small(__m256i *c_s, const __m256i *a_s, const __m256i *b_small) {
    const __m256i c0_s = _mm256_add_epi64(*a_s, *b_small);
    const __m256i mask_ = _mm256_cmpgt_epi32(*a_s, c0_s);
    const __m256i corr_ = _mm256_srli_epi64(mask_, 32);  // corr=P_n when a_s > c0_s
    *c_s = _mm256_add_epi64(c0_s, corr_);
}

inline void PoseidongoldElement_sub_avx_s_b_small(__m256i *c_s, const __m256i *a_s, const __m256i *b) {
    const __m256i c0_s = _mm256_sub_epi64(*a_s, *b);
    const __m256i mask_ = _mm256_cmpgt_epi32(c0_s, *a_s);
    const __m256i corr_ = _mm256_srli_epi64(mask_, 32);  // corr=P_n when a_s > c0_s
    *c_s = _mm256_sub_epi64(c0_s, corr_);
}

inline void PoseidongoldElement_square_avx(__m256i *c, __m256i *a) {
    __m256i c_h, c_l;
    PoseidongoldElement_square_avx_128(&c_h, &c_l, a);
    PoseidongoldElement_reduce_avx_128_64(c, &c_h, &c_l);
}

inline void PoseidongoldElement_square_avx_128(__m256i *c_h, __m256i *c_l, const __m256i *a) {
    __m256i a_h = _mm256_castps_si256(_mm256_movehdup_ps(_mm256_castsi256_ps(*a)));

    __m256i c_hh = _mm256_mul_epu32(a_h, a_h);
    __m256i c_lh = _mm256_mul_epu32(*a, a_h);  // used as 2^c_lh
    __m256i c_ll = _mm256_mul_epu32(*a, *a);

    __m256i c_ll_h = _mm256_srli_epi64(c_ll, 33);  // yes 33, low part of 2*c_lh is [31:0]
    __m256i r0 = _mm256_add_epi64(c_lh, c_ll_h);

    __m256i r0_l = _mm256_slli_epi64(r0, 33);
    __m256i c_ll_l = _mm256_and_si256(c_ll, sqmask);
    *c_l = _mm256_add_epi64(r0_l, c_ll_l);

    __m256i r0_h = _mm256_srli_epi64(r0, 31);
    *c_h = _mm256_add_epi64(c_hh, r0_h);
}

inline void PoseidongoldElement_mult_avx(__m256i *c, const __m256i *a, const __m256i *b) {
    __m256i c_h, c_l;
    PoseidongoldElement_mult_avx_128(&c_h, &c_l, a, b);
    PoseidongoldElement_reduce_avx_128_64(c, &c_h, &c_l);
}

inline void PoseidongoldElement_mult_avx_72(__m256i *c_h, __m256i *c_l, const __m256i *a, const __m256i *b) {
    __m256i a_h = _mm256_srli_epi64(*a, 32);

    __m256i c_hl = _mm256_mul_epu32(a_h, *b);
    __m256i c_ll = _mm256_mul_epu32(*a, *b);

    __m256i c_ll_h = _mm256_srli_epi64(c_ll, 32);
    __m256i r0 = _mm256_add_epi64(c_hl, c_ll_h);

    __m256i r0_l = _mm256_slli_epi64(r0, 32);
    *c_l = _mm256_blend_epi32(c_ll, r0_l, 0xaa);
    *c_h = _mm256_srli_epi64(r0, 32);
}

inline void PoseidongoldElement_mult_avx_128(__m256i *c_h, __m256i *c_l, const __m256i *a, const __m256i *b) {
    __m256i a_h = _mm256_castps_si256(_mm256_movehdup_ps(_mm256_castsi256_ps(*a)));
    __m256i b_h = _mm256_castps_si256(_mm256_movehdup_ps(_mm256_castsi256_ps(*b)));

    __m256i c_hh = _mm256_mul_epu32(a_h, b_h);
    __m256i c_hl = _mm256_mul_epu32(a_h, *b);
    __m256i c_lh = _mm256_mul_epu32(*a, b_h);
    __m256i c_ll = _mm256_mul_epu32(*a, *b);

    __m256i c_ll_h = _mm256_srli_epi64(c_ll, 32);
    __m256i r0 = _mm256_add_epi64(c_hl, c_ll_h);

    __m256i r0_l = _mm256_and_si256(r0, P_n);
    __m256i r1 = _mm256_add_epi64(c_lh, r0_l);

    __m256i r1_l = _mm256_castps_si256(_mm256_moveldup_ps(_mm256_castsi256_ps(r1)));
    *c_l = _mm256_blend_epi32(c_ll, r1_l, 0xaa);

    __m256i r0_h = _mm256_srli_epi64(r0, 32);
    __m256i r2 = _mm256_add_epi64(c_hh, r0_h);

    __m256i r1_h = _mm256_srli_epi64(r1, 32);
    *c_h = _mm256_add_epi64(r2, r1_h);
}

inline void PoseidongoldElement_mmult_avx(__m256i *a0, __m256i *a1, __m256i *a2, const PoseidongoldElement M[144]) {
    __m256i b0, b1, b2;
    PoseidongoldElement_mmult_avx_4x12(&b0, a0, a1, a2, &(M[0]));
    PoseidongoldElement_mmult_avx_4x12(&b1, a0, a1, a2, &(M[48]));
    PoseidongoldElement_mmult_avx_4x12(&b2, a0, a1, a2, &(M[96]));
    *a0 = b0;
    *a1 = b1;
    *a2 = b2;
}

inline void PoseidongoldElement_mmult_avx_4x12(__m256i *b, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement M[48]) {
    __m256i r0, r1, r2, r3;
    PoseidongoldElement_spmv_avx_4x12(&r0, a0, a1, a2, &(M[0]));
    PoseidongoldElement_spmv_avx_4x12(&r1, a0, a1, a2, &(M[12]));
    PoseidongoldElement_spmv_avx_4x12(&r2, a0, a1, a2, &(M[24]));
    PoseidongoldElement_spmv_avx_4x12(&r3, a0, a1, a2, &(M[36]));

    __m256i t0 = _mm256_permute2f128_si256(r0, r2, 0b00100000);
    __m256i t1 = _mm256_permute2f128_si256(r1, r3, 0b00100000);
    __m256i t2 = _mm256_permute2f128_si256(r0, r2, 0b00110001);
    __m256i t3 = _mm256_permute2f128_si256(r1, r3, 0b00110001);
    __m256i c0 = _mm256_castpd_si256(_mm256_unpacklo_pd(_mm256_castsi256_pd(t0), _mm256_castsi256_pd(t1)));
    __m256i c1 = _mm256_castpd_si256(_mm256_unpackhi_pd(_mm256_castsi256_pd(t0), _mm256_castsi256_pd(t1)));
    __m256i c2 = _mm256_castpd_si256(_mm256_unpacklo_pd(_mm256_castsi256_pd(t2), _mm256_castsi256_pd(t3)));
    __m256i c3 = _mm256_castpd_si256(_mm256_unpackhi_pd(_mm256_castsi256_pd(t2), _mm256_castsi256_pd(t3)));

    __m256i sum0, sum1;
    PoseidongoldElement_add_avx(&sum0, &c0, &c1);
    PoseidongoldElement_add_avx(&sum1, &c2, &c3);
    PoseidongoldElement_add_avx(b, &sum0, &sum1);
}

inline void PoseidongoldElement_mmult_avx_8(__m256i *a0, __m256i *a1, __m256i *a2, const PoseidongoldElement M_8[144]) {
    __m256i b0, b1, b2;
    PoseidongoldElement_mmult_avx_4x12_8(&b0, a0, a1, a2, &(M_8[0]));
    PoseidongoldElement_mmult_avx_4x12_8(&b1, a0, a1, a2, &(M_8[48]));
    PoseidongoldElement_mmult_avx_4x12_8(&b2, a0, a1, a2, &(M_8[96]));
    *a0 = b0;
    *a1 = b1;
    *a2 = b2;
}

inline void PoseidongoldElement_mmult_avx_4x12_8(__m256i *b, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement M_8[48]) {
    __m256i r0, r1, r2, r3;
    PoseidongoldElement_spmv_avx_4x12_8(&r0, a0, a1, a2, &(M_8[0]));
    PoseidongoldElement_spmv_avx_4x12_8(&r1, a0, a1, a2, &(M_8[12]));
    PoseidongoldElement_spmv_avx_4x12_8(&r2, a0, a1, a2, &(M_8[24]));
    PoseidongoldElement_spmv_avx_4x12_8(&r3, a0, a1, a2, &(M_8[36]));

    __m256i t0 = _mm256_permute2f128_si256(r0, r2, 0b00100000);
    __m256i t1 = _mm256_permute2f128_si256(r1, r3, 0b00100000);
    __m256i t2 = _mm256_permute2f128_si256(r0, r2, 0b00110001);
    __m256i t3 = _mm256_permute2f128_si256(r1, r3, 0b00110001);
    __m256i c0 = _mm256_castpd_si256(_mm256_unpacklo_pd(_mm256_castsi256_pd(t0), _mm256_castsi256_pd(t1)));
    __m256i c1 = _mm256_castpd_si256(_mm256_unpackhi_pd(_mm256_castsi256_pd(t0), _mm256_castsi256_pd(t1)));
    __m256i c2 = _mm256_castpd_si256(_mm256_unpacklo_pd(_mm256_castsi256_pd(t2), _mm256_castsi256_pd(t3)));
    __m256i c3 = _mm256_castpd_si256(_mm256_unpackhi_pd(_mm256_castsi256_pd(t2), _mm256_castsi256_pd(t3)));

    __m256i sum0, sum1;
    PoseidongoldElement_add_avx(&sum0, &c0, &c1);
    PoseidongoldElement_add_avx(&sum1, &c2, &c3);
    PoseidongoldElement_add_avx(b, &sum0, &sum1);
}

inline void PoseidongoldElement_spmv_avx_4x12(__m256i *c, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement b[12]) {
    __m256i b0, b1, b2;
    PoseidongoldElement_load_avx(&b0, &(b[0]));
    PoseidongoldElement_load_avx(&b1, &(b[4]));
    PoseidongoldElement_load_avx(&b2, &(b[8]));

    __m256i c0, c1, c2;
    PoseidongoldElement_mult_avx(&c0, a0, &b0);
    PoseidongoldElement_mult_avx(&c1, a1, &b1);
    PoseidongoldElement_mult_avx(&c2, a2, &b2);

    __m256i c_;
    PoseidongoldElement_add_avx(&c_, &c0, &c1);
    PoseidongoldElement_add_avx(c, &c_, &c2);
}

inline void PoseidongoldElement_spmv_avx_4x12_8(__m256i *c, const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement b_8[12]) {
    __m256i b0, b1, b2;
    PoseidongoldElement_load_avx(&b0, &(b_8[0]));
    PoseidongoldElement_load_avx(&b1, &(b_8[4]));
    PoseidongoldElement_load_avx(&b2, &(b_8[8]));

    __m256i c0_h, c1_h, c2_h;
    __m256i c0_l, c1_l, c2_l;
    PoseidongoldElement_mult_avx_72(&c0_h, &c0_l, a0, &b0);
    PoseidongoldElement_mult_avx_72(&c1_h, &c1_l, a1, &b1);
    PoseidongoldElement_mult_avx_72(&c2_h, &c2_l, a2, &b2);

    __m256i c_h, c_l, aux_h, aux_l;

    PoseidongoldElement_add_avx(&aux_l, &c0_l, &c1_l);
    PoseidongoldElement_add_avx(&c_l, &aux_l, &c2_l);

    aux_h = _mm256_add_epi64(c0_h, c1_h);  // do with epi32?
    c_h = _mm256_add_epi64(aux_h, c2_h);

    PoseidongoldElement_reduce_avx_96_64(c, &c_h, &c_l);
}

inline void PoseidongoldElement_reduce_avx_96_64(__m256i *c, const __m256i *c_h, const __m256i *c_l) {
    __m256i c1 = _mm256_mul_epu32(*c_h, P_n);          // c_hl*P_n (only 32bits of c_h useds)
    PoseidongoldElement_add_avx_b_small(c, c_l, &c1);  // c1 = c_hl*P_n <= (2^32-1)*(2^32-1) <= 2^64 -2^33+1 < P
}

inline void PoseidongoldElement_reduce_avx_128_64(__m256i *c, const __m256i *c_h, const __m256i *c_l) {
    __m256i c_hh = _mm256_srli_epi64(*c_h, 32);
    __m256i c1_s, c_ls, c_s;
    PoseidongoldElement_shift_avx(&c_ls, c_l);
    PoseidongoldElement_sub_avx_s_b_small(&c1_s, &c_ls, &c_hh);
    __m256i c2 = _mm256_mul_epu32(*c_h, P_n);  // c_hl*P_n (only 32bits of c_h useds)
    PoseidongoldElement_add_avx_s_b_small(&c_s, &c1_s, &c2);
    PoseidongoldElement_shift_avx(c, &c_s);
}

inline void PoseidongoldElement_toCanonical_avx_s(__m256i *a_sc, const __m256i *a_s) {
    // a_s < P_s iff a < P. Then iff a >= P the mask bits are 0
    __m256i mask1_ = _mm256_cmpgt_epi64(P_s, *a_s);
    __m256i corr1_ = _mm256_andnot_si256(mask1_, P_n);
    *a_sc = _mm256_add_epi64(*a_s, corr1_);
}

#endif  // AVX2 guard
