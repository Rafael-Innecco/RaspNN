#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

#define FLOAT_MAX 0X7F800000

// Some utility funcitons

float32_t* init_matrix(const float32_t x, const int m, const int n);

float32_t* init_matrix_random(const int m, const int n);

void print_matrix(float32_t*, int, int);

void print_int_matrix(int*, int, int);

// Matrix operations

// Tested
float32_t* sum_matrix(const float32_t* A, const float32_t* B, const int m,
                      const int n);

float32_t* sum_matrix_vector(const float32_t* A, const float32_t* B,
                             const int m, const int n);

// Tested
float32_t* diff_matrix(const float32_t* A, const float32_t* B, const int m,
                       const int n);

// Tested
void sum_multiply_matrix_scalar_fast(float32_t* A, const float32_t* B,
                                     const float32_t x, const int m,
                                     const int n);
// Tested
float32_t* relu_matrix(const float32_t* A, const int m, const int n);

// Tested
float32_t* relu_derivate_matrix(const float32_t* A, const int m, const int n);

// Tested
float32_t max_vector_fast(float32_t* A, const int n);

// Tested
float32_t min_vector_fast(float32_t* A, const int n);

// Tested
float32_t* minmax_matrix(const float32_t* A, const int m, const int n);

// Tested
void copy_vector(const float32_t* A, float32_t* B, const int n);

// Tested
float32_t* transpose_matrix(const float32_t* A, const int m, const int n);

// Tested
float32_t* one_hot_matrix(const int* A, const int m, const int n);

// Tested
float32_t* multiply_matrix_scalar(const float32_t* A, const float32_t x,
                                  const int m, const int n);

// Tested
float32_t* multiply_matrix_matrix(const float32_t* A, const float32_t* B,
                                  const int m, const int l, const int n);

float32_t* multiply_matrix_hadamard(const float32_t* A, const float32_t* B,
                                    const int m, const int n);

// Tested
int* compare_vector(const int* A, const int* B, const int n);

// Tested
float32_t* matrix_redux_float(const float32_t* A, const int n, const int m);

// Tested
int* matrix_redux_int(const int* A, const int n, const int m);

#endif  // MATRIXNEON_H
