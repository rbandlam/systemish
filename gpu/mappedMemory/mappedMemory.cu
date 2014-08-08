#include "common.h"

int volatile *h_A, *h_B;
int volatile *d_A, *d_B;
int volatile *h_flag_1, *d_flag_1;
int volatile *h_flag_2, *d_flag_2;

pthread_t thread;

__global__ void
vectorAdd(volatile int *A, volatile int *B, volatile int *flag_1, volatile int *flag_2, int N)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int iter = 0;
	
	if(i < N) {
		// Wait for CPU flag a finite number of times
		for(iter = 0; iter < ITERS; iter ++) {
			while(flag_1[0] == 0) {
				// Wait for host flag to be raised
			}

			// Lower host flag
			flag_1[0] = 0;

			B[i] = A[i] * A[i];

			// Raise device flag
			flag_2[0] = 1;
		}
	}
}

void *gpu_run(void *ptr)
{
	// Full execution measurements
	long long start_cycles = 0, end_cycles = 0, tot_cycles = 0;

	int i = 0, j = 0;

	assert(NUM_PKTS < 64);			// Use one block

	for(i = 0; i < ITERS; i ++) {
		printf("Iteration %d\n", i);

		start_cycles = get_cycles();
		
		for(j = 0; j < NUM_PKTS; j ++) {
			h_A[j] = (i & 0xff) + j + 1;		// Always > 0
			assert(h_A[j] != 0);
		}

		// Raise host flag
		h_flag_1[0] = 1;

		while(h_flag_2[0] == 0) {
			// Wait for device flag
		}

		// Lower device flag
		h_flag_2[0] = 0;

		for(j = 0; j < NUM_PKTS; j ++) {
			int kernel_inp = (i & 0xff) + j + 1;
			// Verify for all iterations except the last iteration
			if(h_B[j] != kernel_inp * kernel_inp) {
				fprintf(stderr, "Kernel output mismatch error\n");
				exit(-1);
			}
		}

		end_cycles = get_cycles();

		tot_cycles += (end_cycles - start_cycles);

		if(rand() % 100 == 0) {
			printf("Iter %d: %d ns\n", i, (int) ((end_cycles - start_cycles) / 2.7));
		}
	}

	printf("\nFull execution stats: %d ns\n", (int) (tot_cycles / (2.7 * ITERS)));

	return 0;
}

int main(void)
{
	int err = cudaSuccess;
	printDeviceProperties();

	cudaSetDeviceFlags(cudaDeviceMapHost);

	// Allocate host vectors as mapped memory
	err = cudaHostAlloc(&h_A, NUM_PKTS * sizeof(int), cudaHostAllocMapped);
	err = cudaHostAlloc(&h_B, NUM_PKTS * sizeof(int), cudaHostAllocMapped);

	// Allocate the host and device flags (host memory versions)
	err = cudaHostAlloc(&h_flag_1, sizeof(int), cudaHostAllocMapped);
	err = cudaHostAlloc(&h_flag_2, sizeof(int), cudaHostAllocMapped);
		
	CPE(err != cudaSuccess, "Could not allocate managed memory\n", -1);

	assert(h_A != NULL);
	assert(h_B != NULL);
	assert(h_flag_1 != NULL);
	assert(h_flag_2 != NULL);

	// Zero out the mapped memory vectors
	h_flag_1[0] = 0;
	h_flag_2[0] = 0;
	for(int j = 0; j < NUM_PKTS; j++)	{
		h_A[j] = 0;
		h_B[j] = 0;
	}

	// Get device pointer for mapped memory
	err = cudaHostGetDevicePointer((void **) &d_A, (void *) h_A, 0);
	err = cudaHostGetDevicePointer((void **) &d_B, (void *) h_B, 0);
	err = cudaHostGetDevicePointer((void **) &d_flag_1, (void *) h_flag_1, 0);
	err = cudaHostGetDevicePointer((void **) &d_flag_2, (void *) h_flag_2, 0);

	CPE(err != cudaSuccess, "Could not get device pointer for mapped memory\n", -1);

	// Launch the CPU code
	pthread_create(&thread, NULL, gpu_run, NULL);

	// Launch the kernel once
	printf("Launching CUDA kernel\n");
	int threadsPerBlock = NUM_PKTS;
	int blocksPerGrid = (NUM_PKTS + threadsPerBlock - 1) / threadsPerBlock;
	cudaStream_t my_stream;
	err = cudaStreamCreate(&my_stream);
	CPE(err != cudaSuccess, "Failed to create cudaStream\n", -1);

	vectorAdd<<<blocksPerGrid, threadsPerBlock, 0, my_stream>>>(d_A, d_B, d_flag_1, d_flag_2, NUM_PKTS);
	cudaStreamQuery(my_stream);

	printf("Waiting for CPU thread to finish\n");
	pthread_join(thread, NULL);

	// Free allocated mapped memory
	cudaFreeHost((void *) h_A);
	cudaFreeHost((void *) h_B);

	// Reset the device and exit
	err = cudaDeviceReset();
	CPE(err != cudaSuccess, "Failed to de-initialize the device\n", -1);

	printf("Done\n");
	return 0;
}

