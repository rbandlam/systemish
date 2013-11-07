#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 8000000
#define SHM_ID 3006

//Value is valid iff beacon is 1
struct atom {
	int value;
	int beacon;
};	


