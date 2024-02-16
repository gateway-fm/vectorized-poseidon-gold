#ifndef POSEIDONGOLD_ELEMENT_SCALAR
#define POSEIDONGOLD_ELEMENT_SCALAR

#include "poseidongold_element.h"
#include "poseidongold_element_utils.h"

PoseidongoldElement PoseidongoldElement_add(const PoseidongoldElement in1, const PoseidongoldElement in2);
void PoseidongoldElement_loadAdd(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2);

PoseidongoldElement PoseidongoldElement_inc(const PoseidongoldElement fe);

PoseidongoldElement PoseidongoldElement_sub(const PoseidongoldElement in1, const PoseidongoldElement in2);
void PoseidongoldElement_loadSub(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2);

PoseidongoldElement PoseidongoldElement_dec(const PoseidongoldElement fe);

PoseidongoldElement PoseidongoldElement_mul(const PoseidongoldElement in1, const PoseidongoldElement in2);
void PoseidongoldElement_loadMul(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2);

PoseidongoldElement PoseidongoldElement_square(const PoseidongoldElement in1);
void PoseidongoldElement_loadSquare(PoseidongoldElement *result, const PoseidongoldElement in1);

PoseidongoldElement PoseidongoldElement_div(const PoseidongoldElement in1, const PoseidongoldElement in2);
void PoseidongoldElement_loadDiv(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2);

PoseidongoldElement PoseidongoldElement_neg(const PoseidongoldElement in1);
void PoseidongoldElement_loadNeg(PoseidongoldElement *result, const PoseidongoldElement in1);

bool PoseidongoldElement_isZero(const PoseidongoldElement in1);

bool PoseidongoldElement_isOne(const PoseidongoldElement in1);

bool PoseidongoldElement_isNegone(const PoseidongoldElement in1);

bool PoseidongoldElement_equal(const PoseidongoldElement in1, const PoseidongoldElement in2);

PoseidongoldElement PoseidongoldElement_inv(const PoseidongoldElement in1);
void PoseidongoldElement_loadInv(PoseidongoldElement *, const PoseidongoldElement);

PoseidongoldElement PoseidongoldElement_mulScalar(const PoseidongoldElement base, const uint64_t scalar);
void PoseidongoldElement_loadMulScalar(PoseidongoldElement *result, const PoseidongoldElement base, const uint64_t scalar);

PoseidongoldElement PoseidongoldElement_exp(PoseidongoldElement base, uint64_t exp);

void PoseidongoldElement_loadExp(PoseidongoldElement *result, PoseidongoldElement base, uint64_t exp);
#endif
