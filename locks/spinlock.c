#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREADS 2

#define NANO 1000000000

typedef struct {
	pthread_spinlock_t lock;
	int index;
	long long pad[7];
} lock_t;

void *kv_function(void *ptr);

lock_t my_lock;

main()
{
	int i;
	pthread_spin_init(&my_lock.lock, 0);
	
	int tid[NUM_THREADS];
	pthread_t thread[NUM_THREADS];
	
	for(i = 0; i < NUM_THREADS; i++) {
		tid[i] = i;
		pthread_create(&thread[i], NULL, kv_function, &tid[i]);
	}

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}

	exit(0);
}

void *kv_function( void *ptr)
{
	int iters = 0;
	struct timespec start, end;
	int tid = *((int *) ptr);
	printf("Starting tid: %d\n", tid);

	clock_gettime(CLOCK_REALTIME, &start);

	while(1) {

		if(iters == 10000000) {
			clock_gettime(CLOCK_REALTIME, &end);
			double seconds = (end.tv_sec - start.tv_sec) +
				(double) (end.tv_nsec - start.tv_nsec) / 1000000000;

			printf("Thread %d: %.2f M /s. Index = %d\n", tid,
				10000000 / (seconds * 1000000), my_lock.index);
	
			iters = 0;	
			clock_gettime(CLOCK_REALTIME, &start);
		}

		pthread_spin_lock(&my_lock.lock);
			
		my_lock.index ++;

		pthread_spin_unlock(&my_lock.lock);

		iters ++;
	}
}
