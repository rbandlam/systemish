#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 1024		//1 M atoms => 4MB
#define NUM_CLIENTS 4
#define PER_CLIENT_CAP (CAPACITY / NUM_CLIENTS)

#define SHM_ID 3006
#define FENCE asm volatile ("" : : : "memory"); \
	asm volatile("mfence" ::: "memory")

//Value is valid iff beacon is 1
struct KV {
	long long key;
	long long value;
	long long req_num;
	long long location;
};	


