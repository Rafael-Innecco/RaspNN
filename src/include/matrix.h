#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

#define FLOAT_MAX 0X7F800000
#define FLOAT_MIN 0xFF800000

float32_t* init_matrix(const float32_t x, const int m, const int n);

float32_t* sum_matrix(const float32_t* A, const float32_t* B, const int m,
                      const int n);

float32_t* diff_matrix(const float32_t* A, const float32_t* B, const int m,
                       const int n);

void sum_multiply_matrix_scalar_fast(float32_t* A, const float32_t* B,
                                     const float32_t x, const int m,
                                     const int n);

float32_t* relu_matrix(const float32_t* A, const int m, const int n);

float32_t* relu_derivate_matrix(const float32_t* A, const int m, const int n);

float32_t* softmax_matrix(float32_t* A, int n, int m);

void minmax_vector_fast(float32_t* A, const int n);

float32_t* minmax_matrix(const float32_t* A, const int m, const int n);

void copy_vector(const float32_t* A, float32_t* B, const int n);

float32_t* transpose_matrix(const float32_t* A, const int m, const int n);

float32_t* one_hot_matrix(const int* A, const int m, const int n);

float32_t* multiply_matrix_scalar(const float32_t* A, const float32_t x,
                                  const int m, const int n);

float32_t* multiply_matrix_matrix(const float32_t* A, const float32_t* B,
                                  const int m, const int l, const int n);

float32_t matrix_redux(float32_t* A, int n, int m);

#endif  // MATRIXNEON_H
