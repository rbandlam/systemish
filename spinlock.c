/*
 * For small critical sections, spinlocks are the better choice.
 * Reason: pthread mutexes result in a context switch.
 **/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *counter_incrementer_function(void *ptr);

//A counter and a mutex to spinlock it
int counter = 0;
int MAX = 5000000;
pthread_spinlock_t counter_spinlock;

main()
{
	pthread_t thread1, thread2, thread3, thread4;
	pthread_spin_init(&counter_spinlock, 0);

	pthread_create(&thread1, NULL, counter_incrementer_function, NULL);
	pthread_create(&thread2, NULL, counter_incrementer_function, NULL);
	pthread_create(&thread3, NULL, counter_incrementer_function, NULL);
	pthread_create(&thread4, NULL, counter_incrementer_function, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

	printf("Counter = %d \n", counter);
	exit(0);
}

void *counter_incrementer_function( void *ptr)
{
	int i;
	for(i = 0; i < MAX; i++) {
		pthread_spin_lock(&counter_spinlock);
		counter++;
		if(counter % 1000000 == 0) {
			printf("%d\n", counter);
		}
		pthread_spin_unlock(&counter_spinlock);
	}
}
