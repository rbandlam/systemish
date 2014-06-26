#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#include "param.h"

struct cache_bkt		/* 64 bytes */
{
	int slot_arr[SLOTS_PER_BKT];
};
struct cache_bkt *cache;

#define ISSET(n, i) (n & (1 << i))	// Is the ith bit of n == 1
#define SET(n, i) (n | (1 << i))	// Is the ith bit of n == 1

#define ABS(a) (a > 0 ? a : -1 * a)

// Prefetch, Save, and Switch
#define PSS(addr, label) \
do {\
	__builtin_prefetch(addr); \
	batch_rips[I] = &&label; \
	I = (I + 1) & BATCH_SIZE_;	\
	goto *batch_rips[I]; \
} while(0)
	 

// Each packet contains a random integer. The memory address accessed
// by the packet is determined by an expensive hash of the integer.
int *pkts;

int sum = 0;

// Process BATCH_SIZE pkts starting from lo
int process_pkts_in_batch(int *pkt_lo)
{
	int I = 0;			// batch index
	void *batch_rips[BATCH_SIZE];		// goto targets
	int iMask = 0;		// No packet is done yet

	int __i[BATCH_SIZE];
	int __jumper[BATCH_SIZE];
	int *__arr[BATCH_SIZE];
	int __j[BATCH_SIZE];
	int __best_j[BATCH_SIZE];
	int __max_diff[BATCH_SIZE];

	int __temp_index;
	for(__temp_index = 0; __temp_index < BATCH_SIZE; __temp_index ++) {
		batch_rips[__temp_index] = &&label_0;
	}

label_0:
	
	__jumper[I] = pkt_lo[I];
			
	for(__i[I] = 0; __i[I] < DEPTH; __i[I] ++) {
		PSS(&cache[__jumper[I]], label_1);
			
label_1:
		__arr[I] = cache[__jumper[I]].slot_arr;
		__best_j[I] = 0;

		__max_diff[I] = ABS(__arr[I][0] - __jumper[I]) % 8;

		for(__j[I] = 1; __j[I] < SLOTS_PER_BKT; __j[I] ++) {
			if(ABS(__arr[I][__j[I]] - __jumper[I]) % 8 > __max_diff[I]) {
				__max_diff[I] = ABS(__arr[I][__j[I]] - __jumper[I]) % 8;
				__best_j[I] = __j[I];
			}
		}
		
		__jumper[I] = __arr[I][__best_j[I]];
		if(__jumper[I] % 16 == 0) {
			break;
		}
	}

	sum += __jumper[I];

end:
	batch_rips[I] = &&end;
	iMask = SET(iMask, I);
	if(iMask == (1 << BATCH_SIZE) - 1) {
		return;
	}
	I = (I + 1) & BATCH_SIZE_;
	goto *batch_rips[I];
}

int main(int argc, char **argv)
{
	int i, j;

	// Allocate a large memory area
	fprintf(stderr, "Size of cache = %lu\n", NUM_BS * sizeof(struct cache_bkt));

	int sid = shmget(CACHE_SID, NUM_BS * sizeof(struct cache_bkt), 
		IPC_CREAT | 0666 | SHM_HUGETLB);
	if(sid < 0) {
		fprintf(stderr, "Could not create cache\n");
		exit(-1);
	}
	cache = shmat(sid, 0, 0);

	// Fill in the cache with index into itself
	for(i = 0; i < NUM_BS; i ++) {
		for(j = 0; j < SLOTS_PER_BKT; j++) {
			cache[i].slot_arr[j] = rand() & NUM_BS_;
		}
	}

	// Allocate the packets
	pkts = (int *) malloc(NUM_PKTS * sizeof(int));
	for(i = 0; i < NUM_PKTS; i++) {
		pkts[i] = rand() & NUM_BS_;
	}

	fprintf(stderr, "Finished creating cache and packets\n");

	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	for(i = 0; i < NUM_PKTS; i += BATCH_SIZE) {
		process_pkts_in_batch(&pkts[i]);
	}

	clock_gettime(CLOCK_REALTIME, &end);
	printf("Time = %f sum = %d\n", 
		(end.tv_sec - start.tv_sec) + (double) (end.tv_nsec - start.tv_nsec) / 1000000000,
		sum);
}
