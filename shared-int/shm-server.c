#include "common.h"
int main()
{
	int shmid = shmget(SHM_ID, sizeof(int), IPC_CREAT | 0666);
	int *write_area = shmat(shmid, 0, 0);

	while(1) {
		*write_area = (rand() / 1000) * 31;
	}
    return 0;
}
