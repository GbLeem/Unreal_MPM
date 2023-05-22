#pragma once

#include <string>
#include "cuda_runtime.h"
#include "vector_types.h"
#include "vector_functions.h"
#include "device_launch_parameters.h"

cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size, std::string* error_message);
cudaError_t addWithCuda2(int4* c, const int4* a, const int4* b, std::string* error_message);
