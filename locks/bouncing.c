/*
 * For small critical sections, spinlocks are the better choice.
 * Reason: pthread mutexes result in a context switch.
 **/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
#include<sched.h>

#define SIZE 1048576
#define SIZE_ 1048575
#define NUM_THREADS 4

#define FENCE asm volatile ("" : : : "memory"); \
	asm volatile("mfence" ::: "memory")

#define NANO 1000000000

struct KV {
	long long location;
	long long value;
	long long req_num;
	long long key;
};

long long A[NUM_THREADS];

struct CC {
	pthread_spinlock_t lock;
	int index;
	long long pad[7];
};

void *kv_function(void *ptr);

struct timespec start[NUM_THREADS], end[NUM_THREADS];


main()
{
	printf("%lu %lu\n", sizeof(struct CC), sizeof(pthread_spinlock_t));
	int i;
	
	int tid[NUM_THREADS];
	pthread_t thread[NUM_THREADS];
	
	for(i = 0; i < NUM_THREADS; i++) {
		A[i] = 0;
		tid[i] = i;
		pthread_create(&thread[i], NULL, kv_function, &tid[i]);
	}
	while(1) {
		sleep(1);
		for(i = 0; i < NUM_THREADS; i++) {
			printf("%lld ", A[i]);
		}
		printf("\n");
	}
	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}

	exit(0);
}

int stick_this_thread_to_core(int core_id) {
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id >= num_cores)  // core_id = 0, 1, ... n-1 if system has n cores
      return EINVAL;

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);

   pthread_t current_thread = pthread_self();    
   return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void *kv_function( void *ptr)
{
	int tid = *((int *) ptr);
	stick_this_thread_to_core(tid);
	printf("Starting tid: %d\n", tid);

	while(1) {
		/*if(A[tid] % 1000000 == 0) {
			clock_gettime(CLOCK_REALTIME, &end[tid]);
			double seconds = (end[tid].tv_sec - start[tid].tv_sec) + 
				((double) (end[tid].tv_nsec - start[tid].tv_nsec) / NANO);
			printf("Thread %d, OPS = %f\n", tid, 1000000 / seconds);
			clock_gettime(CLOCK_REALTIME, &start[tid]);
		}*/
		A[tid] ++;
		if(A[tid] % 100000000 == 0) {
			printf("A[%d] is %lld\n", tid, A[tid]);
		}
	}
}
