#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

float32_t * sum_matrix(float32_t* A, float32_t* B, const int m, const int n);

float32_t * relu_matrix(const float32_t * A, const int m, const int n);

float32_t * softmax_matrix(float32_t * A, int n, int m);

void minmax_vector(float32_t * operand, float32_t * destination, int n);

float32_t * minmax_matrix(float32_t * A, int n, int m);

float32_t * one_hot_matrix(float32_t** A, int n, int m);

float32_t * multiply_matrix_scalar(const float32_t * A, const float32_t x, const int m, const int n);

float32_t * multiply_matrix_matrix(const float32_t * A, const float32_t * B, const int l,
                                     const int m, const int n);

float32_t matrix_redux(float32_t * A, int n, int m);

#endif  // MATRIXNEON_H
