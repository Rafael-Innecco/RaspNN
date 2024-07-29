
#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

float32_t* sum_matrix(float32_t* A, float32_t* B, const int m, const int n);

float32x4_t** relu_matrix(const float32x4_t** A, const int m, const int n);

float32x4_t** softmax_matrix(float32x4_t** A, int n, int m);

float32x4_t** one_hot_matrix(float32x4_t** A, int n, int m);

float32x4_t** multiply_matrix_scalar(const float32x4_t** A, const float32_t x, const int m, const int n);

float32x4_t** multiply_matrix_matrix(const float32x4_t** A, const float32x4_t** B, const int l,
                                     const int m, const int n);

float32x4_t matrix_redux(float32x4_t** A, int n, int m);

#endif  // MATRIXNEON_H
