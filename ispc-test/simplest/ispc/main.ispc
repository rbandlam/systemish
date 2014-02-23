#include "common.h"

export void simple(uniform float data_arr[], uniform int base_index) {
    foreach (iter = 0 ... CAP) {
		for(int j = 0; j < 8; j ++) {			
			data_arr[iter] *= 1.001;
		}
    }
}
