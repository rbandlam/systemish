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

	// Per stage measurements
	long long start_cycles_h2d = 0, start_cycles_kernel = 0, start_cycles_d2h = 0;
	long long end_cycles_h2d = 0, end_cycles_kernel = 0, end_cycles_d2h = 0;
	long long tot_cycles_h2d = 0, tot_cycles_kernel = 0, tot_cycles_d2h = 0;

	// Full execution measurements
	long long start_cycles = 0, end_cycles = 0, tot_cycles = 0;

	int i = 0;

	int threadsPerBlock = 256;
	int blocksPerGrid = (NUM_PKTS + threadsPerBlock - 1) / threadsPerBlock;

	err = cudaStreamCreate(&stream_1);
	CPE(err != cudaSuccess, "Failed to create cudaStream\n", -1);

	err = cudaMalloc((void **) &d_A, NUM_PKTS * sizeof(int));
	CPE(err != cudaSuccess, "Failed to cudaMalloc\n", -1);

	// Measure host-to-device memcpy latency
	for(i = 0; i < ITERS; i ++) {
		start_cycles = get_cycles();
		
		// Stage 1: host to device memcpy
		start_cycles_h2d = get_cycles();
		cudaMemcpyAsync(d_A, h_A, NUM_PKTS * sizeof(int), cudaMemcpyHostToDevice, stream_1);
		end_cycles_h2d = get_cycles();

		// Stage 2: kernel execution
		start_cycles_kernel = get_cycles();
		vectorAdd<<<blocksPerGrid, threadsPerBlock, 0, stream_1>>>(d_A, NUM_PKTS);
		end_cycles_kernel = get_cycles();

		// Stage 2: device to host memcpy
		start_cycles_d2h = get_cycles();
		cudaMemcpyAsync(h_A, d_A, NUM_PKTS * sizeof(int), cudaMemcpyDeviceToHost, stream_1);
		end_cycles_d2h = get_cycles();

		// Complete full execution: the time for this is not included in per-stage measurement
		cudaStreamSynchronize(stream_1);

		end_cycles = get_cycles();

		tot_cycles_h2d += (end_cycles_h2d - start_cycles_h2d);
		tot_cycles_kernel += (end_cycles_kernel - start_cycles_kernel);
		tot_cycles_d2h += (end_cycles_d2h - start_cycles_d2h);
		tot_cycles += (end_cycles - start_cycles);

		if(rand() % 100 == 0) {
			printf("%d | h2d = %d ns, kernel = %d ns, d2h = %d ns, full = %d ns\n", i, 
				(int) ((end_cycles_h2d - start_cycles_h2d) / 2.7),
				(int) ((end_cycles_kernel - start_cycles_kernel) / 2.7),
				(int) ((end_cycles_d2h - start_cycles_d2h) / 2.7),
				(int) ((end_cycles - start_cycles) / 2.7));
		}
		
		err = cudaGetLastError();
		CPE(err != cudaSuccess, "Fail!\n", -1);
	}

	cudaFree(d_A);

	printf("\nFull execution stats:\n");
	printf("\th2d = %d ns, kernel = %d ns, d2h = %d ns, full execution = %d ns\n", 
		(int) (tot_cycles_h2d / (2.7 * ITERS)),
		(int) (tot_cycles_kernel / (2.7 * ITERS)),
		(int) (tot_cycles_d2h / (2.7 * ITERS)), 
		(int) (tot_cycles / (2.7 * ITERS)));

	long long total_busy_cycles = tot_cycles_h2d + tot_cycles_kernel + tot_cycles_d2h;
	printf("\nSynchronization time = %d ns\n", 
		(int) ((tot_cycles - total_busy_cycles) / (2.7 * ITERS)));

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

