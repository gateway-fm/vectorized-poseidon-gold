#include "poseidongold_hash_avx2.h"

#ifdef __AVX2__

inline void PoseidongoldHash_pow7_avx(__m256i *st0, __m256i *st1, __m256i *st2) {
    __m256i pw2_0, pw2_1, pw2_2;
    PoseidongoldElement_square_avx(&pw2_0, st0);
    PoseidongoldElement_square_avx(&pw2_1, st1);
    PoseidongoldElement_square_avx(&pw2_2, st2);
    __m256i pw4_0, pw4_1, pw4_2;
    PoseidongoldElement_square_avx(&pw4_0, &pw2_0);
    PoseidongoldElement_square_avx(&pw4_1, &pw2_1);
    PoseidongoldElement_square_avx(&pw4_2, &pw2_2);
    __m256i pw3_0, pw3_1, pw3_2;
    PoseidongoldElement_mult_avx(&pw3_0, &pw2_0, st0);
    PoseidongoldElement_mult_avx(&pw3_1, &pw2_1, st1);
    PoseidongoldElement_mult_avx(&pw3_2, &pw2_2, st2);

    PoseidongoldElement_mult_avx(st0, &pw3_0, &pw4_0);
    PoseidongoldElement_mult_avx(st1, &pw3_1, &pw4_1);
    PoseidongoldElement_mult_avx(st2, &pw3_2, &pw4_2);
};

inline void PoseidongoldHash_add_avx(__m256i *st0, __m256i *st1, __m256i *st2, const PoseidongoldElement C_[SPONGE_WIDTH]) {
    __m256i c0, c1, c2;
    PoseidongoldElement_load_avx(&c0, &(C_[0]));
    PoseidongoldElement_load_avx(&c1, &(C_[4]));
    PoseidongoldElement_load_avx(&c2, &(C_[8]));
    PoseidongoldElement_add_avx(st0, st0, &c0);
    PoseidongoldElement_add_avx(st1, st1, &c1);
    PoseidongoldElement_add_avx(st2, st2, &c2);
}

inline void PoseidongoldHash_add_avx_small(__m256i *st0, __m256i *st1, __m256i *st2, const PoseidongoldElement C_small[SPONGE_WIDTH]) {
    __m256i c0, c1, c2;
    PoseidongoldElement_load_avx(&c0, &(C_small[0]));
    PoseidongoldElement_load_avx(&c1, &(C_small[4]));
    PoseidongoldElement_load_avx(&c2, &(C_small[8]));

    PoseidongoldElement_add_avx_b_small(st0, st0, &c0);
    PoseidongoldElement_add_avx_b_small(st1, st1, &c1);
    PoseidongoldElement_add_avx_b_small(st2, st2, &c2);
}

inline PoseidongoldElement PoseidongoldHash_dot_avx(const __m256i *a0, const __m256i *a1, const __m256i *a2, const PoseidongoldElement b[12]) {
    __m256i c_;
    PoseidongoldElement_spmv_avx_4x12(&c_, a0, a1, a2, b);
    PoseidongoldElement c[4];
    PoseidongoldElement_store_avx(c, &c_);
    return PoseidongoldElement_add(PoseidongoldElement_add(c[0], c[1]), PoseidongoldElement_add(c[2], c[3]));
}

#endif  // AVX2 guard
