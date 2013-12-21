//Non-blocking named pipes
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 268435456
#define SIZE_ 268435455
#define NUM 1000000000

int *hash_table;

int main(int argc, char *argv[]) 
{
	int i, ans;
	hash_table = (int *) malloc(SIZE * sizeof(int));
	for(i = 0; i < SIZE / 4; i++) {
		hash_table[i] = 1;
	}
	struct timespec start, end;
	
	printf("starting %d\n", atoi(argv[1]));
	clock_gettime(CLOCK_REALTIME, &start);
	for(i = 0; i < NUM; i++) {
		ans += hash_table[rand() & SIZE_];
	}
	clock_gettime(CLOCK_REALTIME, &end);
	double seconds = (end.tv_sec - start.tv_sec) + 
		((double) (end.tv_nsec - start.tv_nsec) / 1000000000);

	printf("Random reads per second = %f\n", NUM / seconds);
	printf("Sum = %d\n", ans);
}
