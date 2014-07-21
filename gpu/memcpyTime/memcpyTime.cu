#include "common.h"

void gpu_run(int *h_A)
{
	int *d_A = NULL;
	int err = cudaSuccess;
	long long startCycles = 0, endCycles = 0, totCycles = 0;
	int i = 0;

	err = cudaMalloc((void **) &d_A, NUM_PKTS * sizeof(int));
	CPE(err != cudaSuccess, "Failed to cudaMalloc\n", -1);

	for(i = 0; i < ITERS; i ++) {
		startCycles = get_cycles();
		err = cudaMemcpy(d_A, h_A, NUM_PKTS * sizeof(int), cudaMemcpyHostToDevice);
		endCycles = get_cycles();
		totCycles += (endCycles - startCycles);
	}

	printf("Averages: cycles = %lld, nanoseconds = %f ns\n", totCycles / ITERS,
		totCycles / (ITERS * 2.7));

	CPE(err != cudaSuccess, "Failed to copy to device memory\n", -1);

	err = cudaFree(d_A);
	CPE(err != cudaSuccess, "Failed to cudaFree\n", -1);
}

int main(void)
{
	int err = cudaSuccess;

	printDeviceProperties();

	// Allocate host vectors
	int *h_A = (int *) malloc(NUM_PKTS * sizeof(int));

	// Verify that allocations succeeded
	if (h_A == NULL) {
		fprintf(stderr, "Failed to allocate host vectors!\n");
		exit(EXIT_FAILURE);
	}

	// Initialize the host input vectors
	for (int i = 0; i < NUM_PKTS; ++i)	{
		h_A[i] = i;
	}

	gpu_run(h_A);
	
	// Free host memory
	free(h_A);

	// Reset the device and exit
	err = cudaDeviceReset();
	CPE(err != cudaSuccess, "Failed to de-initialize the device\n", -1);

	printf("Done\n");
	return 0;
}

