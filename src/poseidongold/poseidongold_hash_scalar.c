#include "poseidongold_hash_scalar.h"

#include <immintrin.h>

inline void PoseidongoldHash_pow7(PoseidongoldElement *x) {
    PoseidongoldElement x2 = PoseidongoldElement_mul(*x, *x);
    PoseidongoldElement x3 = PoseidongoldElement_mul(*x, x2);
    PoseidongoldElement x4 = PoseidongoldElement_mul(x2, x2);
    *x = PoseidongoldElement_mul(x3, x4);
}

inline void PoseidongoldHash_pow7_(PoseidongoldElement *x) {
    PoseidongoldElement x2[SPONGE_WIDTH], x3[SPONGE_WIDTH], x4[SPONGE_WIDTH];
    for (int i = 0; i < SPONGE_WIDTH; ++i) {
        x2[i] = PoseidongoldElement_mul(x[i], x[i]);
        x3[i] = PoseidongoldElement_mul(x[i], x2[i]);
        x4[i] = PoseidongoldElement_mul(x2[i], x2[i]);
        x[i] = PoseidongoldElement_mul(x3[i], x4[i]);
    }
}

inline void PoseidongoldHash_add_(PoseidongoldElement *x, const PoseidongoldElement C[SPONGE_WIDTH]) {
    for (int i = 0; i < SPONGE_WIDTH; ++i) {
        x[i] = PoseidongoldElement_add(x[i], C[i]);
    }
}

inline void PoseidongoldHash_pow7add_(PoseidongoldElement *x, const PoseidongoldElement C[SPONGE_WIDTH]) {
    PoseidongoldElement x2[SPONGE_WIDTH], x3[SPONGE_WIDTH], x4[SPONGE_WIDTH];
    for (int i = 0; i < SPONGE_WIDTH; ++i) {
        x2[i] = PoseidongoldElement_mul(x[i], x[i]);
        x3[i] = PoseidongoldElement_mul(x[i], x2[i]);
        x4[i] = PoseidongoldElement_mul(x2[i], x2[i]);
        x[i] = PoseidongoldElement_mul(x3[i], x4[i]);
        x[i] = PoseidongoldElement_add(x[i], C[i]);
    }
}

// rick: check transpose access to matrix
inline void PoseidongoldHash_mvp_(PoseidongoldElement *state, const PoseidongoldElement mat[SPONGE_WIDTH][SPONGE_WIDTH]) {
    PoseidongoldElement old_state[SPONGE_WIDTH];
    memcpy(old_state, state, sizeof(PoseidongoldElement) * SPONGE_WIDTH);

    for (int i = 0; i < SPONGE_WIDTH; i++) {
        state[i] = PoseidongoldElement_mul(mat[0][i], old_state[0]);
        for (int j = 1; j < SPONGE_WIDTH; j++) {
            state[i] = PoseidongoldElement_add(state[i], PoseidongoldElement_mul(mat[j][i], old_state[j]));
        }
    }
}

inline PoseidongoldElement PoseidongoldHash_dot_(PoseidongoldElement *x, const PoseidongoldElement C[SPONGE_WIDTH]) {
    PoseidongoldElement s0 = PoseidongoldElement_mul(x[0], C[0]);
    for (int i = 1; i < SPONGE_WIDTH; i++) {
        s0 = PoseidongoldElement_add(s0, PoseidongoldElement_mul(x[i], C[i]));
    }
    return s0;
}

inline void PoseidongoldHash_prod_(PoseidongoldElement *x, const PoseidongoldElement alpha, const PoseidongoldElement C[SPONGE_WIDTH]) {
    for (int i = 0; i < SPONGE_WIDTH; ++i) {
        x[i] = PoseidongoldElement_mul(alpha, C[i]);
    }
}
