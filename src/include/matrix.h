
#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

float32x4_t* sum_matrix(float32x4_t* A, float32x4_t* B, int n, int m);

float32x4_t* relu_matrix(float32x4_t* A, int n, int m);

float32x4_t* softmax_matrix(float32x4_t* A, int n, int m);

float32x4_t* one_hot_matrix(float32x4_t* A, int n, int m);

float32x4_t* multiply_matrix_scalar(float32x4_t* A, float x, int n, int m);

float32x4_t* multiply_matrix_matrix(float32x4_t* A, float32x4_t* B, int n, int k, int m);

float32_t matrix_redux(float32x4_t* A, int n, int m);

#endif // MATRIXNEON_H
