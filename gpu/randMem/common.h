#include <stdio.h>
#include <cuda_runtime.h>
#include <time.h>

#define DEPTH 4

#define NUM_PKTS (8 * 1024 * 1024)
#define LOG_CAP (64 * 1024 * 1024)		// 256 M log

void printDeviceProperties();

#define CPE(val, msg, err_code) \
	if(val) { fprintf(stderr, msg); fprintf(stderr, " Error %d \n", err_code); \
	exit(err_code);}
