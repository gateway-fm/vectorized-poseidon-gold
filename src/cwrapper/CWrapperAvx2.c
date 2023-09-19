#include "CWrapper.h"

void hashAvx2(unsigned long long *input, unsigned long long *capacity, unsigned long long *result) {
    const Goldilocks::Element goldilocksInput[12] = {
        Goldilocks::fromU64(input[0]),
        Goldilocks::fromU64(input[1]),
        Goldilocks::fromU64(input[2]),
        Goldilocks::fromU64(input[3]),
        Goldilocks::fromU64(input[4]),
        Goldilocks::fromU64(input[5]),
        Goldilocks::fromU64(input[6]),
        Goldilocks::fromU64(input[7]),
        Goldilocks::fromU64(capacity[0]),
        Goldilocks::fromU64(capacity[1]),
        Goldilocks::fromU64(capacity[2]),
        Goldilocks::fromU64(capacity[3]),
    };
    Goldilocks::Element goldilocksResult[12] = {
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

    PoseidonGoldilocks::hash_full_result(goldilocksResult, goldilocksInput);

    result[0] = Goldilocks::toU64(goldilocksResult[0]);
    result[1] = Goldilocks::toU64(goldilocksResult[1]);
    result[2] = Goldilocks::toU64(goldilocksResult[2]);
    result[3] = Goldilocks::toU64(goldilocksResult[3]);
}
