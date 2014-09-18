#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM 1000000

int main(int argc, char *argv[]) 
{
	int sum = 0, i;
	struct timespec start, end;
	
	clock_gettime(CLOCK_REALTIME, &start);
	for(i = 0; i < NUM; i++) {
		sum += lrand48() % 7;
	}
	clock_gettime(CLOCK_REALTIME, &end);

	double ns = (end.tv_sec - start.tv_sec) * 1000000000 + 
		(end.tv_nsec - start.tv_nsec);

	printf("Time per rand = %.2f ns\n", ns / NUM);
}
