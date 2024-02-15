#include "poseidongold_hash.h"

#ifdef __AVX2__
bool PoseidongoldHash_initializedAvx2 = false;
#endif  // AVX2 guard

#ifdef __AVX512F__
bool PoseidongoldHash_initializedAvx512 = false;
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

#ifdef __AVX512F__

void PoseidongoldHash_result_avx512(PoseidongoldElement *state, const PoseidongoldElement *input) {
    if (!PoseidongoldHash_initializedAvx512) {
        PoseidongoldHash_initializedAvx512 = true;
        PoseidongoldElement_initializeAvx512();
    }

    const int length = 2 * SPONGE_WIDTH * sizeof(PoseidongoldElement);
    memcpy(state, input, length);
    __m512i st0, st1, st2;

    PoseidongoldElement_load_avx512(&st0, &(state[0]));
    PoseidongoldElement_load_avx512(&st1, &(state[8]));
    PoseidongoldElement_load_avx512(&st2, &(state[16]));
    PoseidongoldHash_add_avx512_small(&st0, &st1, &st2, &(PoseidongoldHash_C[0]));

    for (int r = 0; r < HALF_N_FULL_ROUNDS - 1; r++) {
        PoseidongoldHash_pow7_avx512(&st0, &st1, &st2);
        PoseidongoldHash_add_avx512_small(&st0, &st1, &st2, &(PoseidongoldHash_C[(r + 1) * SPONGE_WIDTH]));  // rick
        PoseidongoldElement_mmult_avx512_8(&st0, &st1, &st2, &(PoseidongoldHash_M_[0]));
    }
    PoseidongoldHash_pow7_avx512(&st0, &st1, &st2);
    PoseidongoldHash_add_avx512(&st0, &st1, &st2, &(PoseidongoldHash_C[(HALF_N_FULL_ROUNDS * SPONGE_WIDTH)]));
    PoseidongoldElement_mmult_avx512(&st0, &st1, &st2, &(PoseidongoldHash_P_[0]));

    PoseidongoldElement_store_avx512(&(state[0]), &st0);
    PoseidongoldElement s04_[2] = {state[0], state[4]};
    PoseidongoldElement s04[2];

    __m512i mask = _mm512_set_epi64(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0);  // rick, not better to define where u use it?
    for (int r = 0; r < N_PARTIAL_ROUNDS; r++) {
        s04[0] = s04_[0];
        s04[1] = s04_[1];
        PoseidongoldHash_pow7(&s04[0]);
        PoseidongoldHash_pow7(&s04[1]);
        s04[0] = PoseidongoldElement_add(s04[0], PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + r]);
        s04[1] = PoseidongoldElement_add(s04[1], PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + r]);
        s04_[0] = PoseidongoldElement_mul(s04[0], PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r]);
        s04_[1] = PoseidongoldElement_mul(s04[1], PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r]);
        st0 = _mm512_and_si512(st0, mask);  // rick, do we need a new one?
        PoseidongoldElement aux[2];
        PoseidongoldHash_dot_avx512(aux, &st0, &st1, &st2, &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r]));
        s04_[0] = PoseidongoldElement_add(s04_[0], aux[0]);
        s04_[1] = PoseidongoldElement_add(s04_[1], aux[1]);
        __m512i scalar1 = _mm512_set_epi64(s04[1], s04[1], s04[1], s04[1], s04[0], s04[0], s04[0], s04[0]);
        __m512i w0, w1, w2;

        const PoseidongoldElement *auxS = &(PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1]);
        __m512i s0 = _mm512_set4_epi64(auxS[3], auxS[2], auxS[1], auxS[0]);
        __m512i s1 = _mm512_set4_epi64(auxS[7], auxS[6], auxS[5], auxS[4]);
        __m512i s2 = _mm512_set4_epi64(auxS[11], auxS[10], auxS[9], auxS[8]);

        PoseidongoldElement_mult_avx512(&w0, &scalar1, &s0);
        PoseidongoldElement_mult_avx512(&w1, &scalar1, &s1);
        PoseidongoldElement_mult_avx512(&w2, &scalar1, &s2);
        PoseidongoldElement_add_avx512(&st0, &st0, &w0);
        PoseidongoldElement_add_avx512(&st1, &st1, &w1);
        PoseidongoldElement_add_avx512(&st2, &st2, &w2);
        s04[0] = PoseidongoldElement_add(s04[0], PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1]);
        s04[1] = PoseidongoldElement_add(s04[1], PoseidongoldHash_S[(SPONGE_WIDTH * 2 - 1) * r + SPONGE_WIDTH - 1]);
    }

    PoseidongoldElement_store_avx512(&(state[0]), &st0);
    state[0] = s04_[0];
    state[4] = s04_[1];
    PoseidongoldElement_load_avx512(&st0, &(state[0]));

    for (int r = 0; r < HALF_N_FULL_ROUNDS - 1; r++) {
        PoseidongoldHash_pow7_avx512(&st0, &st1, &st2);
        PoseidongoldHash_add_avx512_small(&st0, &st1, &st2, &(PoseidongoldHash_C[(HALF_N_FULL_ROUNDS + 1) * SPONGE_WIDTH + N_PARTIAL_ROUNDS + r * SPONGE_WIDTH]));
        PoseidongoldElement_mmult_avx512_8(&st0, &st1, &st2, &(PoseidongoldHash_M_[0]));
    }
    PoseidongoldHash_pow7_avx512(&st0, &st1, &st2);
    PoseidongoldElement_mmult_avx512_8(&st0, &st1, &st2, &(PoseidongoldHash_M_[0]));

    PoseidongoldElement_store_avx512(&(state[0]), &st0);
    PoseidongoldElement_store_avx512(&(state[8]), &st1);
    PoseidongoldElement_store_avx512(&(state[16]), &st2);
}

#endif  // AVX512 guard
