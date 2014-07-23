#include "common.h"

__global__ void
vectorAdd(int *A, int N)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		A[i] *= A[i];
	}
}

void gpu_run(int *h_A)
{
	cudaStream_t stream_1;
		
	int *d_A = NULL;
	int err = cudaSuccess;
	long long startCycles = 0, endCycles = 0, totCycles = 0;
	int i = 0;

	int threadsPerBlock = 256;
	int blocksPerGrid = (NUM_PKTS + threadsPerBlock - 1) / threadsPerBlock;

	err = cudaStreamCreate(&stream_1);
	CPE(err != cudaSuccess, "Failed to create cudaStream\n", -1);

	err = cudaMalloc((void **) &d_A, NUM_PKTS * sizeof(int));
	CPE(err != cudaSuccess, "Failed to cudaMalloc\n", -1);

	// Measure host-to-device memcpy latency
	for(i = 0; i < ITERS; i ++) {
		startCycles = get_cycles();

		cudaMemcpyAsync(d_A, h_A, NUM_PKTS * sizeof(int), cudaMemcpyHostToDevice, stream_1);

		vectorAdd<<<blocksPerGrid, threadsPerBlock, 0, stream_1>>>(d_A, NUM_PKTS);

		cudaMemcpyAsync(h_A, d_A, NUM_PKTS * sizeof(int), cudaMemcpyDeviceToHost, stream_1);

		cudaStreamSynchronize(stream_1);

		endCycles = get_cycles();
		totCycles += (endCycles - startCycles);

		if(rand() % 100 == 0) {
			printf("%d: Single execution time: %f ns\n", i, (endCycles - startCycles) / 2.7);
		}
		
		err = cudaGetLastError();
		CPE(err != cudaSuccess, "Fail!\n", -1);
	}


	cudaFree(d_A);

	printf("Full execution stats:\n");
	printf("\tcycles = %lld, nanoseconds = %f ns\n\n", totCycles / ITERS,
		totCycles / (ITERS * 2.7));

}

int main(void)
{
	int *h_A;
	int err = cudaSuccess;

	printDeviceProperties();

	// Allocate host vectors
	err = cudaMallocHost((void **) &h_A, NUM_PKTS * sizeof(int));
	CPE(err != cudaSuccess, "Could not allocate pinned memory\n", -1);

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
	
	// Free pinned host memory
	cudaFreeHost(h_A);

	// Reset the device and exit
	err = cudaDeviceReset();
	CPE(err != cudaSuccess, "Failed to de-initialize the device\n", -1);

	printf("Done\n");
	return 0;
}

