#include "poseidongold_gointerface.h"

void poseidongoldHash(unsigned long long *input, unsigned long long *capacity, unsigned long long *result) {
#ifdef __AVX512F__
    PoseidongoldElement goldilocksInput[24];
    PoseidongoldElement goldilocksResult[24];
    memcpy(goldilocksInput, input, 4 * sizeof(unsigned long long));
    memcpy(goldilocksInput + 8, input + 4, 4 * sizeof(unsigned long long));
    memcpy(goldilocksInput + 16, capacity, 4 * sizeof(unsigned long long));
    memset(goldilocksResult, 0, 24 * sizeof(unsigned long long));
#else
    PoseidongoldElement goldilocksInput[12];
    PoseidongoldElement goldilocksResult[12];
    memcpy(goldilocksInput, input, 8 * sizeof(unsigned long long));
    memcpy(goldilocksInput + 8, capacity, 4 * sizeof(unsigned long long));
    memset(goldilocksResult, 0, 12 * sizeof(unsigned long long));
#endif

#ifdef __AVX512F__
    PoseidongoldHash_result_avx512(goldilocksResult, goldilocksInput);
#elif __AVX2__
    PoseidongoldHash_result_avx2(goldilocksResult, goldilocksInput);
#else
    PoseidongoldHash_result_scalar(goldilocksResult, goldilocksInput);
#endif

    result[0] = PoseidongoldElement_toU64(goldilocksResult[0]);
    result[1] = PoseidongoldElement_toU64(goldilocksResult[1]);
    result[2] = PoseidongoldElement_toU64(goldilocksResult[2]);
    result[3] = PoseidongoldElement_toU64(goldilocksResult[3]);
}
