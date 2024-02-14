#ifdef __AVX512__

#include "poseidongold_hash_avx512.h"

inline void PoseidongoldHash_pow7_avx512(__m512i *st0, __m512i *st1, __m512i *st2) {
    __m512i pw2_0, pw2_1, pw2_2;
    PoseidongoldElement_square_avx512(&pw2_0, st0);
    PoseidongoldElement_square_avx512(&pw2_1, st1);
    PoseidongoldElement_square_avx512(&pw2_2, st2);
    __m512i pw4_0, pw4_1, pw4_2;
    PoseidongoldElement_square_avx512(&pw4_0, &pw2_0);
    PoseidongoldElement_square_avx512(&pw4_1, &pw2_1);
    PoseidongoldElement_square_avx512(&pw4_2, &pw2_2);
    __m512i pw3_0, pw3_1, pw3_2;
    PoseidongoldElement_mult_avx512(&pw3_0, &pw2_0, st0);
    PoseidongoldElement_mult_avx512(&pw3_1, &pw2_1, st1);
    PoseidongoldElement_mult_avx512(&pw3_2, &pw2_2, st2);

    PoseidongoldElement_mult_avx512(st0, &pw3_0, &pw4_0);
    PoseidongoldElement_mult_avx512(st1, &pw3_1, &pw4_1);
    PoseidongoldElement_mult_avx512(st2, &pw3_2, &pw4_2);
};

inline void PoseidongoldHash_add_avx512(__m512i *st0, __m512i *st1, __m512i *st2, const PoseidongoldElement C_[SPONGE_WIDTH]) {
    __m512i c0 = _mm512_set4_epi64(C_[3], C_[2], C_[1], C_[0]);
    __m512i c1 = _mm512_set4_epi64(C_[7], C_[6], C_[5], C_[4]);
    __m512i c2 = _mm512_set4_epi64(C_[11], C_[10], C_[9], C_[8]);
    PoseidongoldElement_add_avx512(st0, st0, &c0);
    PoseidongoldElement_add_avx512(st1, st1, &c1);
    PoseidongoldElement_add_avx512(st2, st2, &c2);
}

inline void PoseidongoldHash_add_avx512_small(__m512i *st0, __m512i *st1, __m512i *st2, const PoseidongoldElement C_small[SPONGE_WIDTH]) {
    __m512i c0 = _mm512_set4_epi64(C_small[3], C_small[2], C_small[1], C_small[0]);
    __m512i c1 = _mm512_set4_epi64(C_small[7], C_small[6], C_small[5], C_small[4]);
    __m512i c2 = _mm512_set4_epi64(C_small[11], C_small[10], C_small[9], C_small[8]);

    PoseidongoldElement_add_avx512_b_c(st0, st0, &c0);
    PoseidongoldElement_add_avx512_b_c(st1, st1, &c1);
    PoseidongoldElement_add_avx512_b_c(st2, st2, &c2);
}

inline void PoseidongoldHash_dot_avx512(PoseidongoldElement c[2], const __m512i *a0, const __m512i *a1, const __m512i *a2, const PoseidongoldElement b[12]) {
    __m512i c_;
    PoseidongoldElement_spmv_avx512_4x12(&c_, a0, a1, a2, b);
    PoseidongoldElement cc[8];
    PoseidongoldElement_store_avx512(cc, &c_);
    c[0] = PoseidongoldElement_add(PoseidongoldElement_add(cc[0], cc[1]), PoseidongoldElement_add(cc[2], cc[3]));
    c[1] = PoseidongoldElement_add(PoseidongoldElement_add(cc[4], cc[5]), PoseidongoldElement_add(cc[6], cc[7]));
}

#endif  // AVX512 guard
