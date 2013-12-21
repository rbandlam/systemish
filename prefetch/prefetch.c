#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define CAP (128 * 1024 * 1024)
int main(int argc, char **argv)
{
	int prefetch = atoi(argv[1]);
	int *A = (int *) malloc(CAP * sizeof(int));
	int *Ind = (int *) malloc(CAP * sizeof(int));

	int i;
	for(i = 0; i < CAP; i++) {
		A[i] = i;
		Ind[i] = rand() % CAP;
	}
	printf("Finished assigning data and indices\n");

	int hi = 0, lo = 0, ans = 0;
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	if(prefetch) {
		while(1) {
			//printf("hi = %d, lo = %d\n", hi, lo);
			if(hi < CAP && hi - lo < 256) {
				int index = Ind[hi];
				//printf("\tPrefetching %d\n", hi);
				__builtin_prefetch(&A[index]);
				hi ++;
			}
			if(hi - lo >= 256 || hi == CAP) {
				//printf("\tAccessing %d\n", lo);
				int index = Ind[lo];
				ans += A[index];
				lo ++;
			}
			if(lo == CAP) {
				break;
			}
		}
		printf("ans = %d\n", ans);
	} else {
		for(lo = 0; lo < CAP; lo ++) {
			int index = Ind[lo];
			ans += A[index];
		}
		printf("ans = %d\n", ans);
	}
	clock_gettime(CLOCK_REALTIME, &end);
	printf("Time = %f\n", (end.tv_sec -start.tv_sec) + 
		(double) (end.tv_nsec - start.tv_nsec) / 1000000000);
	
	
	clock_gettime(CLOCK_REALTIME, &start);
	i = 2;
	for(lo = 0; lo < CAP; lo ++) {
		i %= 1000000;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	printf("Time = %f\n", (end.tv_sec -start.tv_sec) + 
		(double) (end.tv_nsec - start.tv_nsec) / 1000000000);
}
