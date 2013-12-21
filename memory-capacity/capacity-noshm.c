#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define SIZE 1073741824
#define SIZE_ 1073741823

#define MEGA 1048576
#define MEGA_ 1048575

int main(int argc, char **argv)
{
	int id = atoi(argv[1]);
	char *A = malloc(SIZE);

	int i;
	for(i = 0; i < SIZE; i++) {
		A[i] = 'c';
	}
	printf("Initialized the random access region for id %d\n", id);
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	int iter = 0;
	while(1) {
		if((iter & MEGA_) == MEGA_ && iter != 0) {
			clock_gettime(CLOCK_REALTIME, &end);
			double seconds = (end.tv_sec - start.tv_sec) +
				(double) (end.tv_nsec - start.tv_nsec) / 1000000000;
			
			printf("OPS for %d = %f\n", id, 1000000 / seconds);
			clock_gettime(CLOCK_REALTIME, &start);
		}
		int j = 0, ans = 0;
		for(j = 0; j < 2; j++) {
			int index = rand() & SIZE_;
			ans += A[index];
		}
		iter ++;
	}
}
