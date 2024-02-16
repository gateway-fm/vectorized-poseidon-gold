#ifndef POSEIDONGOLD_SCALAR
#define POSEIDONGOLD_SCALAR

#include "poseidongold_element_inc.h"
#include "poseidongold_hash_utils.h"

void PoseidongoldHash_pow7(PoseidongoldElement *x);
void PoseidongoldHash_pow7_(PoseidongoldElement *x);
void PoseidongoldHash_add_(PoseidongoldElement *x, const PoseidongoldElement C[SPONGE_WIDTH]);
void PoseidongoldHash_pow7add_(PoseidongoldElement *x, const PoseidongoldElement C[SPONGE_WIDTH]);
void PoseidongoldHash_mvp_(PoseidongoldElement *state, const PoseidongoldElement mat[SPONGE_WIDTH][SPONGE_WIDTH]);
PoseidongoldElement PoseidongoldHash_dot_(PoseidongoldElement *x, const PoseidongoldElement C[SPONGE_WIDTH]);
void PoseidongoldHash_prod_(PoseidongoldElement *x, const PoseidongoldElement alpha, const PoseidongoldElement C[SPONGE_WIDTH]);

#endif
