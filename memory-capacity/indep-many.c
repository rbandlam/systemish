// Measure the performance of a large number of independent memory accesses
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define LOG_CAP (128 * 1024 * 1024)
#define LOG_CAP_ (LOG_CAP - 1)

#define NUM_ACCESSES (32 * 1024 * 1024)

int *ht_log;
struct timespec start, end;

void init_ht_log(int tid)
{
	int i;
	int shm_id = shmget(tid, LOG_CAP * sizeof(int), IPC_CREAT | 0666 | SHM_HUGETLB);
	printf("Initializing %lu bytes\n", LOG_CAP * sizeof(int));
	if(shm_id == -1) {
		fprintf(stderr, "shmget Error! Failed to create array\n");
		exit(0);
	}

	ht_log = shmat(shm_id, 0, 0);

	srand(41);
	for(i = 0; i < LOG_CAP; i++) {
		ht_log[i] = rand() & 3;
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
	struct timespec start, end;
	int tid = atoi(argv[1]);
	init_ht_log(tid);
	
	int *ind = (int *) malloc(NUM_ACCESSES * sizeof(int));
	for(i = 0; i < NUM_ACCESSES; i++) {
		ind[i] = rand() & LOG_CAP_;
	}
	
	int iter = 0;
	while(1) {
		iter ++;
		
		clock_gettime(CLOCK_REALTIME, &start);
	
		for(i = 0; i < NUM_ACCESSES; i ++) {
			sum += ht_log[ind[i]];
		}
	
		clock_gettime(CLOCK_REALTIME, &end);
		double seconds = (double) (end.tv_nsec - start.tv_nsec) / 1000000000 + 
			(end.tv_sec - start.tv_sec);
		
		printf("TPUT %d = %f, sum = %d\n", tid, NUM_ACCESSES / seconds, sum);
	}
}
