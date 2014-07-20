#include "common.h"

__global__ void
vectorAdd(int *pkts, const int *log)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < NUM_PKTS) {
		int j;
		for(j = 0; j < DEPTH; j ++) {
			pkts[i] = log[pkts[i]];
		}
	}
}

void cpu_run(int *pkts, int *log)
{
	int i;
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	for(i = 0; i < NUM_PKTS; i ++) {
		int j;
		for(j = 0; j < DEPTH; j ++) {
			pkts[i] = log[pkts[i]];
		}
	}

	clock_gettime(CLOCK_REALTIME, &end);
	double time = (double) (end.tv_nsec - start.tv_nsec) / 1000000000 + 
		(end.tv_sec - start.tv_sec);
	printf("CPU time = %f\n", time);
}

void gpu_run(int *h_pkts, int *h_log)
{
	struct timespec start, end;
	int *d_pkts = NULL, *d_log = NULL;
	int err = cudaSuccess;

	err = cudaMalloc((void **) &d_pkts, NUM_PKTS * sizeof(int));
	err = cudaMalloc((void **) &d_log, LOG_CAP * sizeof(int));
	CPE(err != cudaSuccess, "Failed to cudaMalloc\n", -1);

	err = cudaMemcpy(d_pkts, h_pkts, NUM_PKTS * sizeof(int), cudaMemcpyHostToDevice);
	err = cudaMemcpy(d_log, h_log, LOG_CAP * sizeof(int), cudaMemcpyHostToDevice);
	CPE(err != cudaSuccess, "Failed to copy to device memory\n", -1);

	// Launch the Vector Add CUDA Kernel
	int threadsPerBlock = 256;
	int blocksPerGrid = (NUM_PKTS + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);

	// Start the clock
	clock_gettime(CLOCK_REALTIME, &start);

	vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_pkts, d_log);
	cudaDeviceSynchronize();

	clock_gettime(CLOCK_REALTIME, &end);
	double time = (double) (end.tv_nsec - start.tv_nsec) / 1000000000 + 
		(end.tv_sec - start.tv_sec);
	printf("GPU time = %f\n", time);

	err = cudaGetLastError();
	CPE(err != cudaSuccess, "Failed to launch vectorAdd kernel\n", -1);

	// Copy back the result
	printf("Copy output data from the CUDA device to the host memory\n");
	err = cudaMemcpy(h_pkts, d_pkts, NUM_PKTS * sizeof(int), cudaMemcpyDeviceToHost);
	CPE(err != cudaSuccess, "Failed to copy C from device to host\n", -1);

	// Free device global memory
	err = cudaFree(d_pkts);
	err = cudaFree(d_log);
	CPE(err != cudaSuccess, "Failed to cudaFree\n", -1);
}

int main(void)
{
	int err = cudaSuccess, i;

	printDeviceProperties();

	int *h_pkts_cpu = (int *) malloc(NUM_PKTS * sizeof(int));
	int *h_pkts_gpu = (int *) malloc(NUM_PKTS * sizeof(int));
	int *h_log = (int *) malloc(LOG_CAP * sizeof(int));

	// Verify that allocations succeeded
	if (h_pkts_cpu == NULL || h_pkts_gpu == NULL || h_log == NULL) {
		fprintf(stderr, "Failed to allocate host mem!\n");
		exit(-1);
	}
	
	// Initialize packets
	for(i = 0; i < NUM_PKTS; i ++) {
		h_pkts_cpu[i] = rand() % LOG_CAP;
		h_pkts_gpu[i] = h_pkts_cpu[i];
	}

	// Initialize log
	for(i = 0; i < LOG_CAP; i ++) {
		h_log[i] = rand() % LOG_CAP;
	}

	cpu_run(h_pkts_cpu, h_log);
	gpu_run(h_pkts_gpu, h_log);

	// Verify that the result vector is correct
	for(int i = 0; i < NUM_PKTS; i ++) {
		if (h_pkts_cpu[i] != h_pkts_gpu[i]) {
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			fprintf(stderr, "CPU %d, GPU %d\n", h_pkts_cpu[i], h_pkts_gpu[i]);
			exit(-1);
		}
	}

	printf("Test PASSED\n");

	// Free host memory
	free(h_pkts_cpu);
	free(h_pkts_gpu);
	free(h_log);

	// Reset the device and exit
	err = cudaDeviceReset();
	CPE(err != cudaSuccess, "Failed to de-initialize the device\n", -1);

	printf("Done\n");
	return 0;
}

