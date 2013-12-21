/*
 * For small critical sections, spinlocks are the better choice.
 * Reason: pthread mutexes result in a context switch.
 **/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define SIZE 1048576
#define SIZE_ 1048575
#define NUM_THREADS 4
#define NUM_MACHINES 1

#define FENCE asm volatile ("" : : : "memory"); \
	asm volatile("mfence" ::: "memory")

#define NANO 1000000000

struct KV {
	long long location;
	long long value;
	long long req_num;
	long long key;
};

struct CC {
	pthread_spinlock_t lock;
	int index;
	long long pad[7];
};

void *kv_function(void *ptr);

struct KV *kv_array[NUM_MACHINES];
struct CC cc_array[NUM_MACHINES];

struct timespec start[NUM_MACHINES], end[NUM_MACHINES];

main()
{
	printf("%lu %lu\n", sizeof(struct CC), sizeof(pthread_spinlock_t));
	int i;
	for(i = 0; i < NUM_MACHINES; i++) {
		kv_array[i] = (struct KV*) malloc(SIZE * sizeof(struct KV));
		cc_array[i].index = 0;
		pthread_spin_init(&cc_array[i].lock, 0);
	}
	
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
	int tid = *((int *) ptr);
	int mid = tid % NUM_MACHINES;
	printf("Starting tid: %d, mid: %d\n", tid, mid);

	while(1) {
		pthread_spin_lock(&cc_array[mid].lock);
			if(cc_array[mid].index == 0) {
				// This scheme works because end and start are global variables.
				// One thread can capture "start" and another the corresponding
				// "end". That is fine.
				clock_gettime(CLOCK_REALTIME, &end[mid]);
				double seconds = (end[mid].tv_sec - start[mid].tv_sec) + 
					((double) (end[mid].tv_nsec - start[mid].tv_nsec) / NANO);
				printf("Machine %d, OPS = %f\n", mid, 1000000 / seconds);
				clock_gettime(CLOCK_REALTIME, &start[mid]);
			}
			/*kv_array[mid][kv_index[mid]].key = kv_index[mid] + 1;
			kv_array[mid][kv_index[mid]].location = kv_index[mid] + 2;
			kv_array[mid][kv_index[mid]].req_num = kv_index[mid] + 3;
			FENCE;
			kv_array[mid][kv_index[mid]].value = kv_index[mid] + 4;*/
			cc_array[mid].index = (cc_array[mid].index + 1) & SIZE_;
		pthread_spin_unlock(&cc_array[mid].lock);
	}
}
