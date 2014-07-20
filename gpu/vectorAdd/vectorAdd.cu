#include "common.h"

__global__ void
vectorAdd(const float *A, const float *B, float *C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < N)
    {
        C[i] = A[i] + B[i];
    }
}

int main(void)
{
	int err, N = 500000;
	struct timespec start, end;

	printDeviceProperties();

	printf("[Vector addition of %d elements]\n", N);

	// Allocate host vectors
	float *h_A = (float *) malloc(N * sizeof(float));
	float *h_B = (float *) malloc(N * sizeof(float));
	float *h_C = (float *) malloc(N * sizeof(float));

	// Verify that allocations succeeded
	if (h_A == NULL || h_B == NULL || h_C == NULL) {
		fprintf(stderr, "Failed to allocate host vectors!\n");
		exit(EXIT_FAILURE);
	}

	// Initialize the host input vectors
	for (int i = 0; i < N; ++i)	{
		h_A[i] = rand() / (float) RAND_MAX;
		h_B[i] = rand() / (float) RAND_MAX;
	}

	// Start the clock
	clock_gettime(CLOCK_REALTIME, &start);

	// Allocate the device input vector A
	float *d_A = NULL, *d_B = NULL, *d_C = NULL;

	err = cudaMalloc((void **)&d_A, N * sizeof(float));
	CPE(err != cudaSuccess, "Failed to allocate d_A\n", -1);

	err = cudaMalloc((void **)&d_B, N * sizeof(float));
	CPE(err != cudaSuccess, "Failed to allocate d_B\n", -1);

	err = cudaMalloc((void **)&d_C, N * sizeof(float));
	CPE(err != cudaSuccess, "Failed to allocate d_C\n", -1);

	// Copy A and B to device memory
	printf("Copy input data from the host memory to the CUDA device\n");

	err = cudaMemcpy(d_A, h_A, N * sizeof(float), cudaMemcpyHostToDevice);
	CPE(err != cudaSuccess, "Failed to copy d_A to device memory\n", -1);

	err = cudaMemcpy(d_B, h_B, N * sizeof(float), cudaMemcpyHostToDevice);
	CPE(err != cudaSuccess, "Failed to copy d_B to device memory\n", -1);

	// Launch the Vector Add CUDA Kernel
	int threadsPerBlock = 256;
	int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);

	vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
	clock_gettime(CLOCK_REALTIME, &end);

	err = cudaGetLastError();
	CPE(err != cudaSuccess, "Failed to launch vectorAdd kernel\n", -1);

	// Copy back the result
	printf("Copy output data from the CUDA device to the host memory\n");
	err = cudaMemcpy(h_C, d_C, N * sizeof(float), cudaMemcpyDeviceToHost);
	CPE(err != cudaSuccess, "Failed to copy C from device to host\n", -1);

	double time = (double) (end.tv_nsec - start.tv_nsec) / 1000000000 + 
		(end.tv_sec - start.tv_sec);
	printf("Time = %f\n", time);

	// Verify that the result vector is correct
	for (int i = 0; i < N; ++i) {
		if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5) {
			fprintf(stderr, "Result verification failed at element %d!\n", i);
			exit(EXIT_FAILURE);
		}
	}

	printf("Test PASSED\n");

	// Free device global memory
	err = cudaFree(d_A);
	CPE(err != cudaSuccess, "Failed to free d_A\n", -1);

	err = cudaFree(d_B);
	CPE(err != cudaSuccess, "Failed to free d_B\n", -1);

	err = cudaFree(d_C);
	CPE(err != cudaSuccess, "Failed to free d_C\n", -1);

	// Free host memory
	free(h_A);
	free(h_B);
	free(h_C);

	// Reset the device and exit
	err = cudaDeviceReset();
	CPE(err != cudaSuccess, "Failed to de-initialize the device\n", -1);

	printf("Done\n");
	return 0;
}

