#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 1073741824
#define SIZE_ 1073741823

#define M_64 67108864

int i;
char *A;
struct timespec start, end;

void init_array()
{
	srand(41);
	printf("Initializing %d bytes\n", SIZE);
	A = malloc(SIZE);
	for(i = 0; i < SIZE; i++) {
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
	int i, sum = 0;
	init_array();
	start_timer();
	
	for(i = 0; i < M_64; i ++) {
		sum += A[rand() & SIZE_];
	}
	printf("Sum = %d\n", sum);	

	end_timer();
}
