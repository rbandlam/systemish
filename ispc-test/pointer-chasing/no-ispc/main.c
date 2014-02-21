#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

int i;
int *data_arr, *pkt_arr;
struct timespec start, end;

void init_array()
{
	printf("Initializing %lu bytes\n", CAP * sizeof(int));
	int shm_id = shmget(3185, CAP * sizeof(int), IPC_CREAT | 0666 | SHM_HUGETLB);

	if(shm_id == -1) {
		fprintf(stderr, "shmget Error! Failed to create array\n");
		exit(0);
	}

	data_arr = (int *) shmat(shm_id, 0, 0);
	pkt_arr = (int *) malloc(NUM_PACKETS * sizeof(int));
	memset(pkt_arr, 0, NUM_PACKETS * sizeof(int));

	srand(41);
	for(i = 0; i < CAP; i++) {
		data_arr[i] = rand() & CAP_;
	}

	printf("Done initializing\n");
}

void start_timer()
{
	printf("Starting timer\n");
	clock_gettime(CLOCK_REALTIME, &start);
}

void end_timer()
{
	clock_gettime(CLOCK_REALTIME, &end);
	double seconds = (end.tv_sec - start.tv_sec) +
		(double) (end.tv_nsec - start.tv_nsec) / 1000000000;
	printf("Time = %f\n", seconds);
}


int main(int argc, char **argv)
{
	int pkt_i, batch_i, i, j;
	init_array();
	start_timer();
	
	for(pkt_i = 0; pkt_i < NUM_PACKETS; pkt_i += BATCH_SIZE) {
		for(batch_i = 0; batch_i < BATCH_SIZE; batch_i ++) {
			int i = pkt_i + batch_i;
			int addr = data_arr[i & CAP_];		// Sequential access into data_arr

			for(j = 0; j < LOOKUPS_PER_PACKET; j ++) {
				pkt_arr[i] += data_arr[addr];
				addr = (data_arr[addr] + j) & CAP_;
			}
		}
	}

	end_timer();
	
	printf("Random sample from pkt_arr: %d\n", pkt_arr[rand() % NUM_PACKETS]);
}
