#include "common.h"
int main()
{
	int shmid = shmget(SHM_ID, CAPACITY * sizeof(struct atom), 0666);
	struct atom *read_area = shmat(shmid, 0, 0);

	int i = 0;
	long long sum = 0;
	for(i = 0; i < CAPACITY; i++) {
		while(read_area[i].beacon != 1) {
		}
		if(read_area[i].value != i)	{
			printf("Error reading %d\n", i);
			exit(0);
		}
		if(i % 1000000 == 0) {
			printf("Client read %d\n", i);
		}
	}
	
    return 0;
}
