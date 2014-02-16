#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

int i;
int *A;
struct timespec start, end;

void init_array()
{
	printf("Initializing %lu bytes\n", CAP * sizeof(int));
	int shm_id = shmget(3185, CAP * sizeof(int), IPC_CREAT | 0666 | SHM_HUGETLB);
	if(shm_id == -1) {
		fprintf(stderr, "shmget Error! Failed to create array\n");
		exit(0);
	}

	A = (int *)shmat(shm_id, 0, 0);

	srand(41);
	for(i = 0; i < CAP; i++) {
		A[i] = rand();
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
	int i, j;
	init_array();
	start_timer();
	
	int sum[BATCH_SIZE], total_sum;
	int ind[BATCH_SIZE];
	memset(sum, 0, BATCH_SIZE * sizeof(int));
	
	for(i = 0; i < NUM_LOOKUPS / BATCH_SIZE; i ++) {
		for(j = 0; j < BATCH_SIZE; j++) {
			ind[j] = rand() & CAP_;
			__builtin_prefetch (&A[ind[j]], 0, 1);
		}

		for(j = 0; j < BATCH_SIZE; j++) {
			sum[j] += A[ind[j]];
		}
	}
	
	for(j = 0; j < BATCH_SIZE; j++) {
		total_sum += sum[j];
	}

	printf("Total Sum = %d\n", total_sum);	

	end_timer();
}
