// Measure the performance of few independent memory accesses
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 268435456
#define SIZE_ 268435455
#define NUM_ACCESSES 32		// Number of independent memory acceses to perform

char *A;
struct timespec start, end;

void init_array()
{
	int i;
	int shm_id = shmget(3185, SIZE, IPC_CREAT | 0666 | SHM_HUGETLB);
	printf("Initializing %d bytes\n", SIZE);
	if(shm_id == -1) {
		fprintf(stderr, "shmget Error! Failed to create array\n");
		exit(0);
	}

	A = shmat(shm_id, 0, 0);

	srand(41);
	for(i = 0; i < SIZE; i++) {
		A[i] = rand() & 3;
	}
	printf("Done initializing\n");
}

inline long long get_cycles()
{
	unsigned low, high;
	unsigned long long val;
	asm volatile ("rdtsc" : "=a" (low), "=d" (high));
	val = high;
	val = (val << 32) | low;
	return val;
}

int main(int argc, char **argv)
{
	int i, sum = 0;
	init_array();
	
	int ind[NUM_ACCESSES];
	for(i = 0; i < NUM_ACCESSES; i++) {
		ind[i] = rand() & SIZE_;
	}
	
	if(A[rand() & SIZE_] == 1) {
		sum = -1;
	}

	long long start_tsc = get_cycles();

	for(i = 0; i < NUM_ACCESSES; i ++) {
		sum += A[ind[i]];
	}

	// Add sum to end_tsc to introduce a dependency between the 
	// memory accesses and the timestamp
	long long end_tsc = get_cycles() + sum;

	printf("Sum = %d nanoseconds = %f\n", 
		sum, (double) (end_tsc - start_tsc) / (2.7 * NUM_ACCESSES));	

}
