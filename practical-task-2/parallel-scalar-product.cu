#include <stdio.h>
#include <math.h>
#include <cuda_runtime.h>
#include <stdlib.h>

#define VECTOR_PART_SIZE 1000
#define PARTS_COUNT 10
#define THREADS_PER_BLOCK 256

/* CUDA kernel для инициализации части вектора */
__global__ void init_vector_part_kernel(int vector_id, int part_number, 
                                        int part_size, double* vector) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < part_size) {
        if (vector_id == 0)
            vector[i] = part_number * part_size + i;
        else
            vector[i] = (part_number * part_size + i) * 0.1;
    }
}

/* CUDA kernel для скалярного произведения части векторов */
__global__ void smulv2_part_kernel(const double* x, const double* y, 
                                    double* partial_results, int part_size) {
    __shared__ double shared_data[THREADS_PER_BLOCK];
    
    int tid = threadIdx.x;
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    
    /* Каждый поток вычисляет один элемент произведения */
    if (i < part_size)
        shared_data[tid] = x[i] * y[i];
    else
        shared_data[tid] = 0.0;
    
    __syncthreads();
    
    /* Редукция в пределах блока */
    for (int stride = blockDim.x / 2; stride > 0; stride >>= 1) {
        if (tid < stride) {
            shared_data[tid] += shared_data[tid + stride];
        }
        __syncthreads();
    }
    
    /* Первый поток записывает результат блока */
    if (tid == 0) {
        partial_results[blockIdx.x] = shared_data[0];
    }
}

/* Инициализация части вектора (обёртка для CUDA) */
void init_vector_part(int vector_id, int part_number, 
                      int part_size, double* vector) {
    int blocks = (part_size + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    init_vector_part_kernel<<<blocks, THREADS_PER_BLOCK>>>(
        vector_id, part_number, part_size, vector);
    cudaDeviceSynchronize();
}

/* Скалярное произведение части векторов (обёртка для CUDA) */
double smulv2_part(const double* x, const double* y, int part_size) {
    int blocks = (part_size + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    int i;
    double result;
    
    /* Выделяем память для частичных результатов на GPU */
    double* d_partial_results;
    cudaMalloc(&d_partial_results, blocks * sizeof(double));
    
    /* Запускаем kernel */
    smulv2_part_kernel<<<blocks, THREADS_PER_BLOCK>>>(x, y, d_partial_results, part_size);
    
    /* Копируем частичные результаты на CPU */
    double* h_partial_results = (double*)malloc(blocks * sizeof(double));
    cudaMemcpy(h_partial_results, d_partial_results, blocks * sizeof(double), 
               cudaMemcpyDeviceToHost);
    
    /* Суммируем частичные результаты на CPU */
    result = 0;
    for (i = 0; i < blocks; i++) {
        result += h_partial_results[i];
    }
    
    /* Освобождаем память */
    free(h_partial_results);
    cudaFree(d_partial_results);
    
    return result;
}

/* Вывод результата */
void print_result(double n) {
    printf("Result: %lf\n", sqrt(n));
}

/* Суммирование массива (редукция) */
double reduce_sum(double* arr, int begin, int end) {
    if (end - begin == 1) {
        return arr[begin];
    }
    if (end - begin > 1) {
        int mid = begin + (end - begin) / 2;
        double tmp1 = reduce_sum(arr, begin, mid);
        double tmp2 = reduce_sum(arr, mid, end);
        return tmp1 + tmp2;
    }
    return 0;
}

/* Умножение векторов по частям */
void mulv(double** x, double** y, double* z) {
    int i;
    for (i = 0; i < PARTS_COUNT; i++) {
        z[i] = smulv2_part(x[i], y[i], VECTOR_PART_SIZE);
    }
}

int main() {
    int i;
    double result;
    
    /* Выделяем память для частей векторов на GPU */
    double** x = (double**)malloc(PARTS_COUNT * sizeof(double*));
    double** y = (double**)malloc(PARTS_COUNT * sizeof(double*));
    double* z = (double*)malloc(PARTS_COUNT * sizeof(double));
    
    /* Инициализируем части векторов x и y на GPU */
    for (i = 0; i < PARTS_COUNT; i++) {
        cudaMalloc(&x[i], VECTOR_PART_SIZE * sizeof(double));
        cudaMalloc(&y[i], VECTOR_PART_SIZE * sizeof(double));
        
        init_vector_part(0, i, VECTOR_PART_SIZE, x[i]);
        init_vector_part(1, i, VECTOR_PART_SIZE, y[i]);
    }
    
    /* Вычисляем скалярное произведение по частям */
    mulv(x, y, z);
    
    /* Суммируем частичные результаты */
    result = reduce_sum(z, 0, PARTS_COUNT);
    
    /* Выводим результат */
    print_result(result);
    
    /* Освобождаем память на GPU */
    for (i = 0; i < PARTS_COUNT; i++) {
        cudaFree(x[i]);
        cudaFree(y[i]);
    }
    free(x);
    free(y);
    free(z);
    
    return 0;
}