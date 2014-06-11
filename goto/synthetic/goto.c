#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#include "param.h"

// Each packet contains a random integer. The memory address accessed
// by the packet is determined by an expensive hash of the integer.

#define SLOTS_PER_BKT 16

struct cache_bkt		/* 64 bytes */
{
	int slot_arr[SLOTS_PER_BKT];
};
struct cache_bkt *cache;

#define CACHE_SID 1
#define NUM_BS (2 * 1024 * 1024)		// Number of cache buckets (avoiding BKTS)
#define NUM_BS_ ((2 * 1024 * 1024) - 1)

int *pkts;
#define NUM_PKTS (1024 * 1024)

#define BATCH_SIZE 8
#define BATCH_SIZE_ 7

#define DEPTH 4

int sum = 0;
int batch_index = 0;

// Process BATCH_SIZE pkts starting from lo
int process_pkts_in_batch(int *pkt_lo)
{
	// Like a foreach loop
	
	int __i[BATCH_SIZE], __jumper[BATCH_SIZE], __j[BATCH_SIZE], __best_j[BATCH_SIZE];
	int __max_diff[BATCH_SIZE];
	struct cache_bkt *__bkt[BATCH_SIZE];

	int batch_index = 0;

label1:		
	__jumper[batch_index] = pkt_lo[batch_index];
			
	for(i = 0; i < DEPTH; i++) {
		__builtin_prefetch(&cache[__jumper[batch_index]]);
		batch_index = (batch_index + 1) & BATCH_SIZE_;
		if(batch_index != 0) {
			goto label1;
		}

		__bkt[batch_index] = &cache[__jumper[batch_index]];
label2:
		int j, best_j = 0;

		int max_diff = bkt->slot_arr[0] - jumper;

		for(j = 1; j < SLOTS_PER_BKT; j ++) {
			if(bkt->slot_arr[j] - jumper > max_diff) {
				max_diff = bkt->slot_arr[j] - jumper;
				best_j = j;
			}
		}
		
		jumper = bkt->slot_arr[best_j];
	}

	sum += jumper;
}

int main(int argc, char **argv)
{
	int i, j;

	// Allocate a large memory area
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
