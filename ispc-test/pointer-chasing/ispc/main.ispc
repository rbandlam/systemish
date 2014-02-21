#include "common.h"

export void simple(uniform int A[]) {
    foreach (i = 0 ... CAP) {
		A[i] ++;
    }
}
