#include "CWrapper.h"

bool initialized = false;

void hashAvx512(unsigned long long *input, unsigned long long *capacity, unsigned long long *result) {
    const Goldilocks::Element goldilocksInput[24] = {
        Goldilocks::fromU64(input[0]),
        Goldilocks::fromU64(input[1]),
        Goldilocks::fromU64(input[2]),
        Goldilocks::fromU64(input[3]),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(input[4]),
        Goldilocks::fromU64(input[5]),
        Goldilocks::fromU64(input[6]),
        Goldilocks::fromU64(input[7]),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(capacity[0]),
        Goldilocks::fromU64(capacity[1]),
        Goldilocks::fromU64(capacity[2]),
        Goldilocks::fromU64(capacity[3]),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
    };
    Goldilocks::Element goldilocksResult[24] = {
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
        Goldilocks::fromU64(0),
    };

    if (!initialized) {
        Goldilocks::initialize_avx512();
        initialized = true;
    }

    PoseidonGoldilocks::hash_full_result_avx512(goldilocksResult, goldilocksInput);

    result[0] = Goldilocks::toU64(goldilocksResult[0]);
    result[1] = Goldilocks::toU64(goldilocksResult[1]);
    result[2] = Goldilocks::toU64(goldilocksResult[2]);
    result[3] = Goldilocks::toU64(goldilocksResult[3]);
}

