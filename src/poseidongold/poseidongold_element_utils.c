#include "poseidongold_element_utils.h"

inline const PoseidongoldElement PoseidongoldElement_zero() { return PoseidongoldElement_ZERO; };
inline void PoseidongoldElement_loadRero(PoseidongoldElement *result) { *result = PoseidongoldElement_ZERO; };

inline const PoseidongoldElement PoseidongoldElement_one() { return PoseidongoldElement_ONE; };
inline void PoseidongoldElement_loadOne(PoseidongoldElement *result) { *result = PoseidongoldElement_ONE; };

inline const PoseidongoldElement PoseidongoldElement_negone() { return PoseidongoldElement_NEGONE; };
inline void PoseidongoldElement_loadNegone(PoseidongoldElement *result) { *result = PoseidongoldElement_NEGONE; };

inline const PoseidongoldElement PoseidongoldElement_shift() { return PoseidongoldElement_SHIFT; };
inline void PoseidongoldElement_loadShift(PoseidongoldElement *result) { *result = PoseidongoldElement_SHIFT; };

inline const PoseidongoldElement PoseidongoldElement_w(uint64_t i) { return PoseidongoldElement_W[i]; };
inline void PoseidongoldElement_loadW(PoseidongoldElement *result, uint64_t i) { *result = PoseidongoldElement_W[i]; };

inline PoseidongoldElement PoseidongoldElement_fromU64(uint64_t in1) {
    PoseidongoldElement res;
    PoseidongoldElement_loadFromU64(&res, in1);
    return res;
}
inline void PoseidongoldElement_loadFromU64(PoseidongoldElement *result, uint64_t in1) { *result = in1; }

inline uint64_t PoseidongoldElement_toU64(const PoseidongoldElement in1) {
    uint64_t res;
    PoseidongoldElement_loadToU64(&res, in1);
    return res;
}
inline void PoseidongoldElement_loadToU64(uint64_t *result, const PoseidongoldElement in1) {
    *result = in1;
    if (*result >= GOLDILOCKS_PRIME) {
        *result -= GOLDILOCKS_PRIME;
    }
}
