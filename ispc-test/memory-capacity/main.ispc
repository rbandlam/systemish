#include "common.h"

export void simple(uniform int A[], uniform int sum[]) {
	varying RNGState rng;
	seed_rng(&rng, programIndex);
    foreach (ispc_i = 0 ... NUM_LOOKUPS) {
		int index = random(&rng) & CAP_;
		//print("programIndex = %d, index = %d SIZE = %d\n", programIndex, index, CAP);
		prefetch_l1(&A[index]);
		sum[programIndex] += A[index];
    }
}
