#include "common.h"

int volatile *h_A, *h_B;
int volatile *d_A, *d_B;

pthread_t thread;

__global__ void
vectorAdd(volatile int *A, volatile int *B, int N)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	
	if(i < N) {
		while(1) {
			while(A[i] == 0) {
				// do nothing
			}
			if(A[i] == 3185) {
				B[i] = A[i] * A[i];
				A[i] = 0;
				break;
			}
			B[i] = A[i] * A[i];
			A[i] = 0;
		}
	}
}

void *gpu_run(void *ptr)
{
	// Per stage measurements
	long long start_cycles_h2d = 0, start_cycles_kernel = 0, start_cycles_d2h = 0;
	long long end_cycles_h2d = 0, end_cycles_kernel = 0, end_cycles_d2h = 0;
	long long tot_cycles_h2d = 0, tot_cycles_kernel = 0, tot_cycles_d2h = 0;

	// Full execution measurements
	long long start_cycles = 0, end_cycles = 0, tot_cycles = 0;

	int i = 0, j = 0;

	assert(NUM_PKTS < 64);			// Use one block

	for(i = 0; i < ITERS; i ++) {
		usleep(200000);
		start_cycles = get_cycles();
		
		// Stage 1: host to device latency
		printf("Making h_A non-zero\n");
		start_cycles_h2d = get_cycles();
		for(j = 0; j < NUM_PKTS; j ++) {
			h_A[j] = (i & 0xff) + j + 1;		// Always > 0

			if(i == ITERS - 1) {
				printf("Last iter: using 3185\n");
				h_A[j] = 3185;
			}

			assert(h_A[j] != 0);
		}
		end_cycles_h2d = get_cycles();

		while(true) {
			int all_zero = true;
			for(j = 0; j < NUM_PKTS; j ++) {
				if(h_A[j] != 0) {
					all_zero = false;
					printf("Iter %d: waiting for element %d to become 0\n", i, j);
					usleep(200000);
				}
			}

			// Check the output if the GPU kernel has processed all h_A
			if(all_zero) {
				break;
			}
		}

		// Stage 2: device to host latency
		start_cycles_d2h = get_cycles();
		for(j = 0; j < NUM_PKTS; j ++) {
			int kernel_inp = (i & 0xff) + j + 1;
			if(i != (ITERS - 1) && h_B[j] != kernel_inp * kernel_inp) {
				fprintf(stderr, "Kernel output mismatch error\n");
				exit(-1);
			}
		}
		end_cycles_d2h = get_cycles();

		end_cycles = get_cycles();

		tot_cycles_h2d += (end_cycles_h2d - start_cycles_h2d);
		tot_cycles_kernel += (end_cycles_kernel - start_cycles_kernel);
		tot_cycles_d2h += (end_cycles_d2h - start_cycles_d2h);
		tot_cycles += (end_cycles - start_cycles);

		if(rand() % 100 == 0) {
			printf("Iter %d | "
				"h2d = %d ns, kernel = %d ns, d2h = %d ns, full = %d ns\n",
				i, 
				(int) ((end_cycles_h2d - start_cycles_h2d) / 2.7),
				(int) ((end_cycles_kernel - start_cycles_kernel) / 2.7),
				(int) ((end_cycles_d2h - start_cycles_d2h) / 2.7),
				(int) ((end_cycles - start_cycles) / 2.7));
		}
	}

	printf("\nFull execution stats:\n");
	printf("\th2d = %d ns, kernel = %d ns, d2h = %d ns, full execution = %d ns\n",
		(int) (tot_cycles_h2d / (2.7 * ITERS)),
		(int) (tot_cycles_kernel / (2.7 * ITERS)),
		(int) (tot_cycles_d2h / (2.7 * ITERS)), 
		(int) (tot_cycles / (2.7 * ITERS)));

	long long total_busy_cycles = tot_cycles_h2d + tot_cycles_kernel + tot_cycles_d2h;
	printf("\nSynchronization time = %d ns\n", 
		(int) ((tot_cycles - total_busy_cycles) / (2.7 * ITERS)));

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
	CPE(err != cudaSuccess, "Could not allocate managed memory\n", -1);

	assert(h_A != NULL);
	assert(h_B != NULL);

	// Zero out the mapped memory vectors
	for(int j = 0; j < NUM_PKTS; j++)	{
		h_A[j] = 0;
		h_B[j] = 0;
	}

	// Get device pointer for mapped memory
	err = cudaHostGetDevicePointer((void **) &d_A, (void *) h_A, 0);
	err = cudaHostGetDevicePointer((void **) &d_B, (void *) h_B, 0);

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

	vectorAdd<<<blocksPerGrid, threadsPerBlock, 0, my_stream>>>(d_A, d_B, NUM_PKTS);
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

