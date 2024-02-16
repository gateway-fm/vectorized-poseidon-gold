#ifndef POSEIDONGOLD_ELEMENT_UTILS
#define POSEIDONGOLD_ELEMENT_UTILS

#include "poseidongold_element.h"

const PoseidongoldElement PoseidongoldElement_zero();
void PoseidongoldElement_loadRero(PoseidongoldElement *result);

const PoseidongoldElement PoseidongoldElement_one();
void PoseidongoldElement_loadOne(PoseidongoldElement *result);

const PoseidongoldElement PoseidongoldElement_negone();
void PoseidongoldElement_loadNegone(PoseidongoldElement *result);

const PoseidongoldElement PoseidongoldElement_shift();
void PoseidongoldElement_loadShift(PoseidongoldElement *result);

const PoseidongoldElement PoseidongoldElement_w(uint64_t i);
void PoseidongoldElement_loadW(PoseidongoldElement *result, uint64_t i);

PoseidongoldElement PoseidongoldElement_fromU64(uint64_t);
void PoseidongoldElement_loadFromU64(PoseidongoldElement *, uint64_t);
uint64_t PoseidongoldElement_toU64(const PoseidongoldElement);
void PoseidongoldElement_loadToU64(uint64_t *, const PoseidongoldElement);

#endif
