#include "cuda_lib_test.h"

__global__ void addKernel(int* c, const int* a, const int* b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

__global__ void addKernel2(int4* c, const int4* a, const int4* b)
{
    int i = threadIdx.x;
    c[i].x = a[i].x + b[i].x;
    c[i].y = a[i].y + b[i].y;
    c[i].z = a[i].z + b[i].z;
    c[i].w = a[i].w + b[i].w;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size, std::string* error_message)
{
    int* dev_a = 0;
    int* dev_b = 0;
    int* dev_c = 0;
    cudaError_t cuda_status;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cuda_status = cudaSetDevice(0);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?";
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cuda_status = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMalloc failed!";
        goto Error;
    }

    cuda_status = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMalloc failed!";
        goto Error;
    }

    cuda_status = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMalloc failed!";
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cuda_status = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMemcpy failed!";
        goto Error;
    }

    cuda_status = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMemcpy failed!";
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel << <1, size >> > (dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cuda_status = cudaGetLastError();
    if (cuda_status != cudaSuccess) {
        *error_message = "addKernel launch failed: " + std::string(cudaGetErrorString(cuda_status));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cuda_status = cudaDeviceSynchronize();
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaDeviceSynchronize returned error code " + std::to_string(cuda_status) + " after launching addKernel!";
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cuda_status = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMemcpy failed!";
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);

    return cuda_status;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda2(int4* c, const int4* a, const int4* b, std::string* error_message)
{
    int4* dev_a = 0;
    int4* dev_b = 0;
    int4* dev_c = 0;
    const unsigned int size = 1;
    cudaError_t cuda_status;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cuda_status = cudaSetDevice(0);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?";
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cuda_status = cudaMalloc((void**)&dev_c, size * sizeof(int4));
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMalloc failed!";
        goto Error;
    }

    cuda_status = cudaMalloc((void**)&dev_a, size * sizeof(int4));
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMalloc failed!";
        goto Error;
    }

    cuda_status = cudaMalloc((void**)&dev_b, size * sizeof(int4));
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMalloc failed!";
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cuda_status = cudaMemcpy(dev_a, a, size * sizeof(int4), cudaMemcpyHostToDevice);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMemcpy failed!";
        goto Error;
    }

    cuda_status = cudaMemcpy(dev_b, b, size * sizeof(int4), cudaMemcpyHostToDevice);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMemcpy failed!";
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel2 << <1, size >> > (dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cuda_status = cudaGetLastError();
    if (cuda_status != cudaSuccess) {
        *error_message = "addKernel launch failed: " + std::string(cudaGetErrorString(cuda_status));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cuda_status = cudaDeviceSynchronize();
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaDeviceSynchronize returned error code " + std::to_string(cuda_status) + " after launching addKernel!";
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cuda_status = cudaMemcpy(c, dev_c, size * sizeof(int4), cudaMemcpyDeviceToHost);
    if (cuda_status != cudaSuccess) {
        *error_message = "cudaMemcpy failed!";
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);

    return cuda_status;
}
