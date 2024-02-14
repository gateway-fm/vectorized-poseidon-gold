#ifndef POSEIDONGOLD_ELEMENT
#define POSEIDONGOLD_ELEMENT

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#define GOLDILOCKS_NUM_ROOTS 33
#define GOLDILOCKS_PRIME 0xFFFFFFFF00000001ULL
#define GOLDILOCKS_PRIME_NEG 0xFFFFFFFF
#define MSB_ 0x8000000000000000  // Most Significant Bit

typedef uint64_t PoseidongoldElement;

extern const PoseidongoldElement PoseidongoldElement_ZR;
extern const PoseidongoldElement PoseidongoldElement_Q;
extern const PoseidongoldElement PoseidongoldElement_MM;
extern const PoseidongoldElement PoseidongoldElement_CQ;
extern const PoseidongoldElement PoseidongoldElement_R2;
extern const PoseidongoldElement PoseidongoldElement_TWO32;

extern const PoseidongoldElement PoseidongoldElement_ZERO;
extern const PoseidongoldElement PoseidongoldElement_ONE;
extern const PoseidongoldElement PoseidongoldElement_NEGONE;
extern const PoseidongoldElement PoseidongoldElement_SHIFT;
extern const PoseidongoldElement PoseidongoldElement_W[GOLDILOCKS_NUM_ROOTS];

#endif
