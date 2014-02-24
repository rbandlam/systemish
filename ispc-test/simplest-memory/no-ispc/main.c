#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include "common.h"

int i;
int *data_arr;
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

	srand(41);
	for(i = 0; i < CAP; i++) {
		data_arr[i] = i;
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

	for(i = 0; i < CAP; i ++) {
		for(j = 0; j < 64; j++) {
			data_arr[i] *= 533;
			data_arr[i] &= CAP_;
		}
	}

	end_timer();
	
	printf("Random sample from data_arr: %d %d\n", data_arr[rand() & CAP_], data_arr[rand() & CAP_]);
}
