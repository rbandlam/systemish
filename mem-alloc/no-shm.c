#include<stdio.h>
#include<stdlib.h>

#define CAP (128 * 1024 * 1024)
int main()
{
	int i, sum = 23;
	int *A = (int *) malloc(CAP * sizeof(int));
	for(i = 0; i < CAP; i++) {
		A[i] = i;
	}
	printf("Random element from A  = %d\n", A[rand() % CAP]);
}
