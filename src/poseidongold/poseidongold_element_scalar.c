#include "poseidongold_element_scalar.h"

inline PoseidongoldElement PoseidongoldElement_add(const PoseidongoldElement in1, const PoseidongoldElement in2) {
    PoseidongoldElement result;
    PoseidongoldElement_loadAdd(&result, in1, in2);
    return result;
}
inline void PoseidongoldElement_loadAdd(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2) {
    __asm__(
        "xor   %%r10, %%r10\n\t"
        "mov   %1, %0\n\t"
        "add   %2, %0\n\t"
        "cmovc %3, %%r10\n\t"
        "add   %%r10, %0\n\t"
        "jnc  1f\n\t"
        "add   %3, %0\n\t"
        "1: \n\t"
        : "=&a"(*result)
        : "r"(in1), "r"(in2), "m"(PoseidongoldElement_CQ), "m"(PoseidongoldElement_ZR)
        : "%r10");
}

inline PoseidongoldElement PoseidongoldElement_inc(const PoseidongoldElement fe) {
    PoseidongoldElement result;
    if (fe < GOLDILOCKS_PRIME - 2) {
        result = fe + 1;
    } else if (fe == GOLDILOCKS_PRIME - 1) {
        result = 0;
    } else {
        result = PoseidongoldElement_add(fe, PoseidongoldElement_one());
    }
    return result;
}

inline PoseidongoldElement PoseidongoldElement_sub(const PoseidongoldElement in1, const PoseidongoldElement in2) {
    PoseidongoldElement result;
    PoseidongoldElement_loadSub(&result, in1, in2);
    return result;
}

inline void PoseidongoldElement_loadSub(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2) {
    __asm__(
        "xor   %%r10, %%r10\n\t"
        "mov   %1, %0\n\t"
        "sub   %2, %0\n\t"
        "cmovc %3, %%r10\n\t"
        "sub   %%r10, %0\n\t"
        "jnc  1f\n\t"
        "sub   %3, %0\n\t"
        "1: \n\t"
        : "=&a"(*result)
        : "r"(in1), "r"(in2), "m"(PoseidongoldElement_CQ), "m"(PoseidongoldElement_ZR)
        : "%r10");
}

inline PoseidongoldElement PoseidongoldElement_dec(const PoseidongoldElement fe) {
    PoseidongoldElement result;
    if (fe > 0) {
        result = fe - 1;
    } else {
        result = GOLDILOCKS_PRIME - 1;
    }
    return result;
}

inline PoseidongoldElement PoseidongoldElement_mul(const PoseidongoldElement in1, const PoseidongoldElement in2) {
    PoseidongoldElement result;
    PoseidongoldElement_loadMul(&result, in1, in2);
    return result;
}

inline void PoseidongoldElement_loadMul(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2) {
    __asm__(
        "mov   %1, %0\n\t"
        "mul   %2\n\t"
        // "xor   %%rbx, %%rbx\n\t"
        "mov   %%edx, %%ebx\n\t"
        "sub   %4, %%rbx\n\t"
        "rol   $32, %%rdx\n\t"
        //"xor   %%rcx, %%rcx;\n\t"
        "mov   %%edx, %%ecx\n\t"
        "sub   %%rcx, %%rdx\n\t"
        "add   %4, %%rcx\n\t"
        "sub   %%rbx, %%rdx\n\t"
        //"mov   %3,%%r10 \n\t"
        "xor   %%rbx, %%rbx\n\t"
        "add   %%rdx, %0\n\t"
        "cmovc %3, %%rbx\n\t"
        "add   %%rbx, %0\n\t"
        // TODO: migrate to labels
        //"xor   %%rbx, %%rbx\n\t"
        //"sub   %%rcx, %0\n\t"
        //"cmovc %%r10, %%rbx\n\t"
        //"sub   %%rbx, %0\n\t"
        "sub   %%rcx, %0\n\t"
        "jnc  1f\n\t"
        "sub   %3, %0\n\t"
        "1: \n\t"
        : "=&a"(*result)
        : "r"(in1), "r"(in2), "m"(PoseidongoldElement_CQ), "m"(PoseidongoldElement_TWO32)
        : "%rbx", "%rcx", "%rdx");
}

inline void PoseidongoldElement_loadMul2(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2) {
    __asm__(
        "mov   %1, %%rax\n\t"
        "mul   %2\n\t"
        "divq   %3\n\t"
        : "=&d"(*result)
        : "r"(in1), "r"(in2), "m"(PoseidongoldElement_Q)
        : "%rax");
}

inline PoseidongoldElement PoseidongoldElement_square(const PoseidongoldElement in1) { return PoseidongoldElement_mul(in1, in1); }

inline void PoseidongoldElement_loadSquare(PoseidongoldElement *result, const PoseidongoldElement in1) { return PoseidongoldElement_loadMul(result, in1, in1); }

inline PoseidongoldElement PoseidongoldElement_div(const PoseidongoldElement in1, const PoseidongoldElement in2) { return PoseidongoldElement_mul(in1, PoseidongoldElement_inv(in2)); }

inline void PoseidongoldElement_loadDiv(PoseidongoldElement *result, const PoseidongoldElement in1, const PoseidongoldElement in2) { PoseidongoldElement_loadMul(result, in1, PoseidongoldElement_inv(in2)); }

inline PoseidongoldElement PoseidongoldElement_neg(const PoseidongoldElement in1) { return PoseidongoldElement_sub(PoseidongoldElement_zero(), in1); }

inline void PoseidongoldElement_loadNeg(PoseidongoldElement *result, const PoseidongoldElement in1) { PoseidongoldElement_loadSub(result, PoseidongoldElement_zero(), in1); }

inline bool PoseidongoldElement_isZero(const PoseidongoldElement in1) { return PoseidongoldElement_equal(in1, PoseidongoldElement_zero()); }

inline bool PoseidongoldElement_isOne(const PoseidongoldElement in1) { return PoseidongoldElement_equal(in1, PoseidongoldElement_one()); }

inline bool PoseidongoldElement_isNegone(const PoseidongoldElement in1) { return PoseidongoldElement_equal(in1, PoseidongoldElement_negone()); }

inline bool PoseidongoldElement_equal(const PoseidongoldElement in1, const PoseidongoldElement in2) { return PoseidongoldElement_toU64(in1) == PoseidongoldElement_toU64(in2); }

inline PoseidongoldElement PoseidongoldElement_inv(const PoseidongoldElement in1) {
    PoseidongoldElement result;
    PoseidongoldElement_loadInv(&result, in1);
    return result;
}

// TODO: Review and optimize inv imlementation
void PoseidongoldElement_loadInv(PoseidongoldElement *result, const PoseidongoldElement in1) {
    if (PoseidongoldElement_isZero(in1)) {
        // std::cerr << "Error: PoseidongoldElement_inv called with zero" << std::endl;
        // exit(-1);
    }
    uint64_t t = 0;
    uint64_t r = GOLDILOCKS_PRIME;
    uint64_t newt = 1;

    uint64_t newr = PoseidongoldElement_toU64(in1);
    PoseidongoldElement q, aux1, aux2;

    while (newr != 0) {
        q = PoseidongoldElement_fromU64(r / newr);
        aux1 = PoseidongoldElement_fromU64(t);
        aux2 = PoseidongoldElement_fromU64(newt);
        t = PoseidongoldElement_toU64(aux2);
        newt = PoseidongoldElement_toU64(PoseidongoldElement_sub(aux1, PoseidongoldElement_mul(q, aux2)));
        aux1 = PoseidongoldElement_fromU64(r);
        aux2 = PoseidongoldElement_fromU64(newr);
        r = PoseidongoldElement_toU64(aux2);
        newr = PoseidongoldElement_toU64(PoseidongoldElement_sub(aux1, PoseidongoldElement_mul(q, aux2)));
    }

    PoseidongoldElement_loadFromU64(result, t);
}

inline PoseidongoldElement PoseidongoldElement_mulScalar(const PoseidongoldElement base, const uint64_t scalar) {
    PoseidongoldElement result;
    PoseidongoldElement_loadMulScalar(&result, base, scalar);
    return result;
}
inline void PoseidongoldElement_loadMulScalar(PoseidongoldElement *result, const PoseidongoldElement base, const uint64_t scalar) {
    PoseidongoldElement eScalar = PoseidongoldElement_fromU64(scalar);
    PoseidongoldElement_loadMul(result, base, eScalar);
}

inline PoseidongoldElement PoseidongoldElement_exp(PoseidongoldElement base, uint64_t exp) {
    PoseidongoldElement result;
    PoseidongoldElement_loadExp(&result, base, exp);
    return result;
}

inline void PoseidongoldElement_loadExp(PoseidongoldElement *result, PoseidongoldElement base, uint64_t exp) {
    *result = PoseidongoldElement_one();

    for (;;) {
        if (exp & 1)
            PoseidongoldElement_loadMul(result, *result, base);
        exp >>= 1;
        if (!exp)
            break;
        PoseidongoldElement_loadMul(&base, base, base);
    }
}
