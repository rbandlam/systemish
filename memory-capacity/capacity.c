#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define SIZE (1024 * 1024 * 1024)
int main()
{
	char *A = malloc(SIZE);
	int i;
	for(i = 0; i < SIZE; i++) {
		A[i] = 'c';
	}
	printf("Initialized the random access region\n");
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	int iter = 0;
	while(1) {
		if(iter % 1000000 == 0 && iter != 0) {
			clock_gettime(CLOCK_REALTIME, &end);
			double seconds = (end.tv_sec - start.tv_sec) +
				(double) (end.tv_nsec - start.tv_nsec) / 1000000000;
			
			printf("OPS = %f\n", 1000000 / seconds);
			clock_gettime(CLOCK_REALTIME, &start);
		}
		int j = 0, ans = 0;
		for(j = 0; j < 2; j++) {
			int index = rand() % SIZE;
			ans += A[index];
		}
		iter ++;
	}
}
