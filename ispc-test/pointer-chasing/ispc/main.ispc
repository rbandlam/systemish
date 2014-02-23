#include "common.h"

export void simple(uniform int data_arr[], uniform int pkt_arr[], uniform int base_index) {
    foreach (iter = 0 ... BATCH_SIZE) {
		int i = base_index + programIndex;
		int addr = data_arr[i & CAP_];

		for(int j = 0; j < LOOKUPS_PER_PACKET; j++) {
			// Memory access
			 addr = data_arr[addr];
			pkt_arr[i] += addr;

			// Computation
			for(int k = 0; k < NUM_COMPUTE_STEPS; k++) {
				addr = (addr + addr + k) & CAP_;
			}
		}
    }
}
