
#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

float16_t * sum_matrix(float16_t* A, float16_t* B, const int m, const int n);

float16_t * relu_matrix(const float16_t * A, const int m, const int n);

float16_t * softmax_matrix(float16_t * A, int n, int m);

float16_t * one_hot_matrix(float16_t** A, int n, int m);

float16_t * multiply_matrix_scalar(const float16_t * A, const float16_t x, const int m, const int n);

float16_t * multiply_matrix_matrix(const float16_t * A, const float16_t * B, const int l,
                                     const int m, const int n);

float16_t matrix_redux(float16_t * A, int n, int m);

#endif  // MATRIXNEON_H
