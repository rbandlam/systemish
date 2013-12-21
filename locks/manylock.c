/*
 * For small critical sections, spinlocks are the better choice.
 * Reason: pthread mutexes result in a context switch.
 **/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#define NUM_THREADS 16
#define NUM_LOCKS 1024

#define NANO 1000000000

struct CC {
	pthread_spinlock_t lock;
	long long pad[7];
};

void *thread_function(void *ptr);

struct CC cc_array[NUM_LOCKS];

main()
{
	printf("%lu %lu\n", sizeof(struct CC), sizeof(pthread_spinlock_t));
	int i;
	for(i = 0; i < NUM_LOCKS; i++) {
		pthread_spin_init(&cc_array[i].lock, 0);
	}
	
	int tid[NUM_THREADS];
	pthread_t thread[NUM_THREADS];
	
	for(i = 0; i < NUM_THREADS; i++) {
		tid[i] = i;
		pthread_create(&thread[i], NULL, thread_function, &tid[i]);
	}

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}

	exit(0);
}

void *thread_function( void *ptr)
{
	int tid = *((int *) ptr);
	printf("Starting tid: %d\nn", tid);
	int iter = 0;
	int SZ = 1024 * 1024 * 1024;
	char *table = (char *) malloc(SZ);
	memset(table, 1, SZ);

	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	while(1) {
		if(iter % 10000 == 0 && iter != 0) {
			clock_gettime(CLOCK_REALTIME, &end);
			double seconds = (end.tv_sec - start.tv_sec) + 
				((double) (end.tv_nsec - start.tv_nsec) / NANO);
			printf("Thread %d, OPS = %f\n", tid, 1000000 / seconds);
			clock_gettime(CLOCK_REALTIME, &start);
		}
		int lock_num = rand() % NUM_LOCKS;
		pthread_spin_lock(&cc_array[lock_num].lock);
		int i, sum = 0;
		for(i = 0; i < 1; i++) {
			sum += table[rand() % SZ];
		}
		pthread_spin_unlock(&cc_array[lock_num].lock);
		iter ++;
	}
}
