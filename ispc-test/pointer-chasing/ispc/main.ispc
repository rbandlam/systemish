#include "common.h"

export void simple(uniform int data_arr[], uniform int pkt_arr[], uniform int base_index) {
    foreach (i = 0 ... BATCH_SIZE) {
		int my_index = base_index + programIndex;
		int addr = data_arr[my_index];
		for(int j = 0; j < LOOKUPS_PER_PACKET; j++) {
			pkt_arr[my_index] += data_arr[addr];
			addr = (data_arr[addr] + j) & CAP_;
		}
    }
}
