#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

#define CAP M_128			//Capacity, in Integers
#define CAP_ M_128_

#define NUM_LOOKUPS M_64

int i;
int *A;
struct timespec start, end;

// Initialize a large "data" array A and a smaller "index" array B
// As B is accessed sequentially, we don't use hugepages for it
void init_array()
{
	printf("Initializing %lu bytes\n", CAP * sizeof(int));
	int shm_id = shmget(3185, CAP * sizeof(int), IPC_CREAT | 0666 | SHM_HUGETLB);

	if(shm_id == -1) {
		fprintf(stderr, "shmget Error! Failed to create array\n");
		exit(0);
	}

	A = (int *) shmat(shm_id, 0, 0);

	srand(41);
	for(i = 0; i < CAP; i++) {
		A[i] = rand() & CAP_;
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
	
	int total_sum, addr = A[0];

	for(i = 0; i < NUM_LOOKUPS; i ++) {
		total_sum += A[addr];
		addr = (A[addr] + i) & CAP_;
	}

	printf("Total Sum = %d\n", total_sum);	

	end_timer();
}
