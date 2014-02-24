#include "common.h"

export void simple(uniform int data_arr[], uniform int base_index) {
    foreach (iter = 0 ... CAP) {
		for(int j = 0; j < 64; j ++) {
			data_arr[iter] *= 533;
			data_arr[iter] &= CAP_;
		}
    }
}
