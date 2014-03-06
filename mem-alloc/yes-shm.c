#include<stdio.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define CAP (128 * 1024 * 1024)
int main()
{
	int i, sum = 23;

	int shmid = shmget(3185, CAP * sizeof(int), IPC_CREAT | 0666);
	if(shmid < 0) {
		fprintf(stderr, "shmget() failed!\n");
		exit(-1);
	}

	int *A = (int *) shmat(shmid, 0, 0);
	for(i = 0; i < CAP; i++) {
		A[i] = i;
	}
	printf("Random element from A  = %d\n", A[rand() % CAP]);
}
