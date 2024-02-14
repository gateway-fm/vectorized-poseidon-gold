#include "poseidongold_hash.h"

#include <immintrin.h>

#ifdef __AVX2__
bool PoseidongoldHash_initializedAvx2 = false;
#endif  // AVX2 guard

void PoseidongoldHash_result_scalar(PoseidongoldElement *state, const PoseidongoldElement *input) {
    const int length = SPONGE_WIDTH * sizeof(PoseidongoldElement);
    memcpy(state, input, length);

    PoseidongoldHash_add_(state, &(PoseidongoldHash_C[0]));
    for (int r = 0; r < HALF_N_FULL_ROUNDS - 1; r++) {
        PoseidongoldHash_pow7add_(state, &(PoseidongoldHash_C[(r + 1) * SPONGE_WIDTH]));
        PoseidongoldHash_mvp_(state, PoseidongoldHash_M);
    }
    PoseidongoldHash_pow7add_(state, &(PoseidongoldHash_C[(HALF_N_FULL_ROUNDS * SPONGE_WIDTH)]));
    PoseidongoldHash_mvp_(state, PoseidongoldHash_P);

    for (int r = 0; r < N_PARTIAL_ROUNDS; r++) {
        PoseidongoldHash_pow7(state + 0);
        state[0] = PoseidongoldElement_add(state[0], PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + r]);
        PoseidongoldElement s0 = PoseidongoldHash_dot_(state, &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r]));
        PoseidongoldElement W_[SPONGE_WIDTH];
        PoseidongoldHash_prod_(W_, state[0], &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1]));
        PoseidongoldHash_add_(state, W_);
        state[0] = s0;
    }

    for (int r = 0; r < HALF_N_FULL_ROUNDS - 1; r++) {
        PoseidongoldHash_pow7add_(state, &(PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + N_PARTIAL_ROUNDS + r * SPONGE_WIDTH]));
        PoseidongoldHash_mvp_(state, PoseidongoldHash_M);
    }
    PoseidongoldHash_pow7_(&(state[0]));
    PoseidongoldHash_mvp_(state, PoseidongoldHash_M);
}

#ifdef __AVX2__

void PoseidongoldHash_result_avx2(PoseidongoldElement *state, const PoseidongoldElement *input) {
    if (!PoseidongoldHash_initializedAvx2) {
        PoseidongoldHash_initializedAvx2 = true;
        PoseidongoldElement_initializeAvx2();
    }

    const int length = SPONGE_WIDTH * sizeof(PoseidongoldElement);
    memcpy(state, input, length);
    __m256i st0, st1, st2;
    PoseidongoldElement_load_avx(&st0, &(state[0]));
    PoseidongoldElement_load_avx(&st1, &(state[4]));
    PoseidongoldElement_load_avx(&st2, &(state[8]));
    PoseidongoldHash_add_avx_small(&st0, &st1, &st2, &(PoseidongoldHash_C[0]));

    for (int r = 0; r < HALF_N_FULL_ROUNDS - 1; r++) {
        PoseidongoldHash_pow7_avx(&st0, &st1, &st2);
        PoseidongoldHash_add_avx_small(&st0, &st1, &st2, &(PoseidongoldHash_C[(r + 1) * SPONGE_WIDTH]));
        PoseidongoldElement_mmult_avx_8(&st0, &st1, &st2, &(PoseidongoldHash_M_[0]));
    }
    PoseidongoldHash_pow7_avx(&st0, &st1, &st2);
    PoseidongoldHash_add_avx(&st0, &st1, &st2, &(PoseidongoldHash_C[(HALF_N_FULL_ROUNDS * SPONGE_WIDTH)]));
    PoseidongoldElement_mmult_avx(&st0, &st1, &st2, &(PoseidongoldHash_P_[0]));

    PoseidongoldElement_store_avx(&(state[0]), &st0);
    PoseidongoldElement state0_ = state[0];
    PoseidongoldElement state0;

    __m256i mask = _mm256_set_epi64x(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0);
    for (int r = 0; r < N_PARTIAL_ROUNDS; r++) {
        state0 = state0_;
        PoseidongoldHash_pow7(&state0);
        state0 = PoseidongoldElement_add(state0, PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + r]);
        state0_ = PoseidongoldElement_mul(state0, PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r]);
        st0 = _mm256_and_si256(st0, mask);
        state0_ = PoseidongoldElement_add(state0_, PoseidongoldHash_dot_avx(&st0, &st1, &st2, &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r])));
        __m256i scalar1 = _mm256_set1_epi64x(state0);
        __m256i w0, w1, w2, s0, s1, s2;
        PoseidongoldElement_load_avx(&s0, &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1]));
        PoseidongoldElement_load_avx(&s1, &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1 + 4]));
        PoseidongoldElement_load_avx(&s2, &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1 + 8]));
        PoseidongoldElement_mult_avx(&w0, &scalar1, &s0);
        PoseidongoldElement_mult_avx(&w1, &scalar1, &s1);
        PoseidongoldElement_mult_avx(&w2, &scalar1, &s2);
        PoseidongoldElement_add_avx(&st0, &st0, &w0);
        PoseidongoldElement_add_avx(&st1, &st1, &w1);
        PoseidongoldElement_add_avx(&st2, &st2, &w2);
        state0 = PoseidongoldElement_add(state0, PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1]);
    }
    PoseidongoldElement_store_avx(&(state[0]), &st0);
    state[0] = state0_;
    PoseidongoldElement_load_avx(&st0, &(state[0]));

    for (int r = 0; r < HALF_N_FULL_ROUNDS - 1; r++) {
        PoseidongoldHash_pow7_avx(&st0, &st1, &st2);
        PoseidongoldHash_add_avx_small(&st0, &st1, &st2, &(PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + N_PARTIAL_ROUNDS + r * SPONGE_WIDTH]));
        PoseidongoldElement_mmult_avx_8(&st0, &st1, &st2, &(PoseidongoldHash_M_[0]));
    }
    PoseidongoldHash_pow7_avx(&st0, &st1, &st2);
    PoseidongoldElement_mmult_avx_8(&st0, &st1, &st2, &(PoseidongoldHash_M_[0]));

    PoseidongoldElement_store_avx(&(state[0]), &st0);
    PoseidongoldElement_store_avx(&(state[4]), &st1);
    PoseidongoldElement_store_avx(&(state[8]), &st2);
}

#endif  // AVX2 guard
