#include "common.h"

export void simple(uniform int A[], uniform int B[], uniform int sum[]) {
    foreach (ispc_i = 0 ... NUM_LOOKUPS) {
		sum[programIndex] += A[B[ispc_i]];
    }
}
