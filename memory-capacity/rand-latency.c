/** < Measure the random-access capacity of a processor */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<assert.h>

#define M_8 (8 * 1024 * 1024)
#define M_8_ ((8 * 1024 * 1024) - 1)

#define LOG_CAP (128 * 1024 * 1024)
#define LOG_CAP_ (LOG_CAP - 1)
#define LOG_KEY 1

int *ht_log;

void init_ht_log()
{
	int i;
	int shm_id = shmget(LOG_KEY, 
		LOG_CAP * sizeof(int), IPC_CREAT | 0666 | SHM_HUGETLB);

	printf("Initializing %lu bytes\n", LOG_CAP * sizeof(int));
	if(shm_id == -1) {
		fprintf(stderr, "shmget Error! Failed to create array\n");
		exit(0);
	}

	ht_log = shmat(shm_id, 0, 0);

	srand(41);
	for(i = 0; i < LOG_CAP; i++) {
		ht_log[i] = rand() & LOG_CAP_;
	}
	printf("Done initializing\n");
}

int main(int argc, char **argv)
{
	int i, sum = 0;
	struct timespec start, end;

	init_ht_log();

	int jumper = rand() & LOG_CAP_;
	clock_gettime(CLOCK_REALTIME, &start);
	
	int iter = 0;
	while(1) {
		iter ++;
		jumper = ht_log[jumper & LOG_CAP_] + iter;

		if((iter & M_8_) == M_8_) {
			clock_gettime(CLOCK_REALTIME, &end);

			double ns = 
				(double) (end.tv_nsec - start.tv_nsec) + 
				(end.tv_sec - start.tv_sec) * 1000000000;
		
			printf("Jumper = %d, latency = %.2f ns\n", 
				jumper, ns / M_8);

			clock_gettime(CLOCK_REALTIME, &start);
		}
	}
}
