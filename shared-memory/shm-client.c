#include "common.h"
int main(int argc, char *argv[])
{
	int id = atoi(argv[1]);
	int shmid = shmget(SHM_ID, CAPACITY * sizeof(struct KV), 0666);
	struct KV *read_area = shmat(shmid, 0, 0);

	long long sum = 0;
	int shm_lo = id * PER_CLIENT_CAP;
	int i = 0;
	while(1) {
		int index = shm_lo + (i % PER_CLIENT_CAP);
		i++;
		sum += read_area[index].key;
		sum += read_area[index].value;
		sum += read_area[index].location;
		sum += read_area[index].req_num;
		/*printf("Client %d read %d, %d\n", id, read_area[index].beacon, 
			read_area[index].value);*/
	}
    return 0;
}
