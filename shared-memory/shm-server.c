#include "common.h"
int main()
{
	int shmid = shmget(SHM_ID, CAPACITY * sizeof(struct KV), IPC_CREAT | 0666);
	struct KV *write_area = shmat(shmid, 0, 0);
	memset(write_area, 0, CAPACITY * sizeof(struct KV));

	int i, iter = 0;
	int shm_cnt[NUM_CLIENTS];
	int shm_lo[NUM_CLIENTS];

	for(i = 0; i < NUM_CLIENTS; i++) {
		shm_cnt[i] = 0;
		shm_lo[i] = i * PER_CLIENT_CAP;
	}

	while(1) {
		for(i = 0; i < NUM_CLIENTS; i++) {
			int index = shm_lo[i] + (shm_cnt[i] % PER_CLIENT_CAP);
			shm_cnt[i] ++;
			write_area[index].key = iter;
			write_area[index].value = iter;
			write_area[index].location = iter;
			FENCE;
			write_area[index].req_num = iter;
		}
		iter ++;
		if(iter % 1000000 == 0) {
			printf("Server completed %d operations\n", 1000000 * NUM_CLIENTS);
		}
	}

    return 0;
}
