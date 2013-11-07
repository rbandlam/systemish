#include "common.h"
int main()
{
	int shmid = shmget(SHM_ID, CAPACITY * sizeof(struct atom), IPC_CREAT | 0666);
	struct atom *write_area = shmat(shmid, 0, 0);
	memset(write_area, 0, CAPACITY * sizeof(struct atom));

	int i = 0;
	for(i = 0; i < CAPACITY; i++) {
		write_area[i].value = i;
		write_area[i].beacon = 1;
		if(i % 1000000 == 0) {
			printf("Server wrote %d\n", i);
		}
		
	}
    return 0;
}
