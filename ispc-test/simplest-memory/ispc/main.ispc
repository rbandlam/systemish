#include "common.h"

export void simple(uniform int data_arr[]) {
    foreach (iter = 0 ... CAP) {
		//int addr = data_arr[iter];
		//#if PREFETCH
		//	prefetch_l1(&data_arr[addr]);
		//#endif

		for(int j = 0; j < 64; j ++) {
			data_arr[iter] *= 533;
			data_arr[iter] &= CAP_;
		}
		//data_arr[iter] += data_arr[addr];
    }
}
