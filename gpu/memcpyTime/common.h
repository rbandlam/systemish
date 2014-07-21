#include <stdio.h>
#include <cuda_runtime.h>
#include <time.h>

#define NUM_PKTS 16 * 1024

void printDeviceProperties();
long long get_cycles();

#define CPE(val, msg, err_code) \
	if(val) { fprintf(stderr, msg); fprintf(stderr, " Error %d \n", err_code); \
	exit(err_code);}
