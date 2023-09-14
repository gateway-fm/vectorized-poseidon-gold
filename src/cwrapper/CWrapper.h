#ifndef EXECUTOR_WRAPPER_H
#define EXECUTOR_WRAPPER_H

#include "../goldilocks/goldilocks_base_field.hpp"
#include "../goldilocks/poseidon_goldilocks.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/*
params are:
input - array with 8 elements
capacity - array with 4 elements
result - array with 4 elements
*/
void hash(unsigned long long *, unsigned long long *, unsigned long long *);

#ifdef __cplusplus
}
#endif

#endif

