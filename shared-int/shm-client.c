#include "common.h"
int main()
{
	int shmid = shmget(SHM_ID, sizeof(int), 0666);
	int *read_area = shmat(shmid, 0, 0);

	sleep(1);
	printf("Client start!\n");
	while(1) {
		int val = *read_area;
		if(val % 31 != 0) {
			printf("Error, read %d\n!", val);
			exit(0);
		}
	}
    return 0;
}
