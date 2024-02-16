#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

const uint64_t PoseidongoldElement_ZR = 0x0000000000000000LL;
const uint64_t PoseidongoldElement_Q = 0xFFFFFFFF00000001LL;
const uint64_t PoseidongoldElement_MM = 0xFFFFFFFeFFFFFFFFLL;
const uint64_t PoseidongoldElement_CQ = 0x00000000FFFFFFFFLL;
const uint64_t PoseidongoldElement_TWO32 = 0x0000000100000000LL;

void addAsm(uint64_t *result, const uint64_t in1, const uint64_t in2) {
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

void subAsm(uint64_t *result, const uint64_t in1, const uint64_t in2) {
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

void mulAsm(uint64_t *result, const uint64_t in1, const uint64_t in2) {
    __asm__(
        "mov   %1, %0\n\t"
        "mul   %2\n\t"
        "mov   %%edx, %%ebx\n\t"
        "sub   %4, %%rbx\n\t"
        "rol   $32, %%rdx\n\t"
        "mov   %%edx, %%ecx\n\t"
        "sub   %%rcx, %%rdx\n\t"
        "add   %4, %%rcx\n\t"
        "sub   %%rbx, %%rdx\n\t"
        "xor   %%rbx, %%rbx\n\t"
        "add   %%rdx, %0\n\t"
        "cmovc %3, %%rbx\n\t"
        "add   %%rbx, %0\n\t"
        "sub   %%rcx, %0\n\t"
        "jnc  1f\n\t"
        "sub   %3, %0\n\t"
        "1: \n\t"
        : "=&a"(*result)
        : "r"(in1), "r"(in2), "m"(PoseidongoldElement_CQ), "m"(PoseidongoldElement_TWO32)
        : "%rbx", "%rcx", "%rdx");
}

void addC(uint64_t *result, const uint64_t in1, const uint64_t in2) {
    bool carry = in1 > UINT64_MAX - in2;
    *result = in1 + in2;
    if (carry) {
        carry = *result > UINT64_MAX - PoseidongoldElement_CQ;
        *result += PoseidongoldElement_CQ;
        if (carry) {
            *result += PoseidongoldElement_CQ;
        }
    }
}

void subC(uint64_t *result, const uint64_t in1, const uint64_t in2) {
    bool carry = in1 < in2;
    *result = in1 - in2;
    if (carry) {
        carry = *result < PoseidongoldElement_CQ;
        *result -= PoseidongoldElement_CQ;
        if (carry) {
            *result -= PoseidongoldElement_CQ;
        }
    }
}

void mulC(uint64_t *result, const uint64_t in1, const uint64_t in2) {
#ifdef __SIZEOF_INT128__
    __uint128_t result128 = ((__uint128_t)in1) * in2;
    *result = result128 & 0xFFFFFFFFFFFFFFFF;
    uint64_t rdx = (result128 >> 64);
#else
    *result = in1 * in2;
    uint64_t lo = in1 * in2;

    uint64_t x0 = in1 & 0xFFFFFFFF;
    uint64_t x1 = in1 >> 32;

    uint64_t y0 = in2 & 0xFFFFFFFF;
    uint64_t y1 = in2 >> 32;

    uint64_t p11 = x1 * y1;
    uint64_t p01 = x0 * y1;
    uint64_t p10 = x1 * y0;
    uint64_t p00 = x0 * y0;

    // 64-bit product + two 32-bit values
    uint64_t middle = p10 + (p00 >> 32) + (p01 & 0xFFFFFFFF);

    // 64-bit product + two 32-bit values
    uint64_t rdx = p11 + (middle >> 32) + (p01 >> 32);
#endif
    uint64_t rbx = rdx & 0xFFFFFFFF;

    rbx -= PoseidongoldElement_TWO32;
    rdx = (rdx << 32) | (rdx >> 32);

    uint64_t rcx = rdx & 0xFFFFFFFF;
    rdx -= rcx;
    rcx += PoseidongoldElement_TWO32;
    rdx -= rbx;

    bool carry = *result > UINT64_MAX - rdx;
    *result += rdx;
    if (carry) {
        *result += PoseidongoldElement_CQ;
    }

    carry = *result < rcx;
    *result -= rcx;
    if (carry) {
        *result -= PoseidongoldElement_CQ;
    }
}

int main() {
    uint64_t result1, result2;

    // no carry check
    addAsm(&result1, 8, 8);
    addC(&result2, 8, 8);
    if (result1 != result2) {
        printf("SUM: Error no carry %lu %lu\n", result1, result2);
        return 1;
    }

    // 1 carry check
    addAsm(&result1, 0xFF00000000000000LL, 0xFF00000000000000LL);
    addC(&result2, 0xFF00000000000000LL, 0xFF00000000000000LL);
    if (result1 != result2) {
        printf("SUM: Error 1 carry %lu %lu\n", result1, result2);
        return 1;
    }

    // 2 carry check
    addAsm(&result1, 0xFFFFFFFFFFFF0000LL, 0xFFFFFFFFFFFF0000LL);
    addC(&result2, 0xFFFFFFFFFFFF0000LL, 0xFFFFFFFFFFFF0000LL);
    if (result1 != result2) {
        printf("SUM: Error 2 carry %lu %lu\n", result1, result2);
        return 1;
    }

    // no carry check
    subAsm(&result1, 16, 8);
    subC(&result2, 16, 8);
    if (result1 != result2) {
        printf("SUB: Error no carry %lu %lu\n", result1, result2);
        return 1;
    }

    // no carry check
    subAsm(&result1, 8, 16);
    subC(&result2, 8, 16);
    if (result1 != result2) {
        printf("SUB: Error 1 carry %lu %lu\n", result1, result2);
        return 1;
    }

    // no carry check
    subAsm(&result1, 8, 0xFFFFFFFFFF000000LL);
    subC(&result2, 8, 0xFFFFFFFFFF000000LL);
    if (result1 != result2) {
        printf("SUB: Error 2 carry %lu %lu\n", result1, result2);
        return 1;
    }

    // use 128bit value
    mulAsm(&result1, 0xFFFFFFFFFF000000LL, 0xFFFFFFFFFF000000LL);
    mulC(&result2, 0xFFFFFFFFFF000000LL, 0xFFFFFFFFFF000000LL);
    if (result1 != result2) {
        printf("MUL: Error with 128bit value %lu (0x%016llx) %lu (0x%016llx)\n", result1, result1, result2, result2);
        return 1;
    }

    // use 128bit value + 2 carry
    mulAsm(&result1, 0xFFFFFFFF00000000LL, 0xFFFFFFFFFF000000LL);
    mulC(&result2, 0xFFFFFFFF00000000LL, 0xFFFFFFFFFF000000LL);
    if (result1 != result2) {
        printf("MUL: Error with 128bit + 2 carry value %lu (0x%016llx) %lu (0x%016llx)\n", result1, result1, result2, result2);
        return 1;
    }

    printf("Success\n");

    return 0;
}
