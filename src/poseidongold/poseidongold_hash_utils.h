#ifndef POSEIDONGOLD_UTILS
#define POSEIDONGOLD_UTILS

#include "poseidongold_element_inc.h"

#define RATE 8
#define CAPACITY 4
#define HASH_SIZE 4
#define SPONGE_WIDTH (RATE + CAPACITY)
#define HALF_N_FULL_ROUNDS 4
#define N_FULL_ROUNDS_TOTAL (2 * HALF_N_FULL_ROUNDS)
#define N_PARTIAL_ROUNDS 22
#define N_ROUNDS (N_FULL_ROUNDS_TOTAL + N_PARTIAL_ROUNDS)

extern const PoseidongoldElement PoseidongoldHash_C[118];
extern const PoseidongoldElement PoseidongoldHash_M[12][12];
extern const PoseidongoldElement PoseidongoldHash_M_[144];
extern const PoseidongoldElement PoseidongoldHash_P[12][12];
extern const PoseidongoldElement PoseidongoldHash_P_[144];
extern const PoseidongoldElement PoseidongoldHash_S[507];

#endif
