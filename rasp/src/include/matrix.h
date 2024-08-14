#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#include <arm_neon.h>

#define FLOAT_MAX 0X7F800000

// Some utility funcitons

void init_matrix(float32_t* A, const float32_t x, const int m, const int n);

void init_matrix_random(float32_t* A, const int m, const int n);

void print_matrix(float32_t*, int, int);

void print_int_matrix(int*, int, int);

void print_matrix_parcial(const float32_t* A, int m, int n, int k);

// Matrix operations

// Tested
void sum_matrix(const float32_t* A, const float32_t* B, float32_t* C, const int m,
                      const int n);

// Tested 
void sum_matrix_vector(const float32_t* A, const float32_t* B, float32_t* C,
                             const int m, const int n);

// Tested
void diff_matrix(const float32_t* A, const float32_t* B, float32_t* C, const int m,
                       const int n);

// Tested
void sum_multiply_matrix_scalar_fast(float32_t* A, const float32_t* B,
                                     const float32_t x, const int m,
                                     const int n);

// Tested
void relu_matrix(const float32_t* A, float32_t* B, const int m, const int n);

// Tested
void relu_derivate_matrix(const float32_t* A, float32_t* B, const int m, const int n);

// Tested
float32_t max_vector_fast(float32_t* A, const int n);

// Tested
float32_t min_vector_fast(float32_t* A, const int n);

// Tested
void minmax_matrix(const float32_t* A, float32_t* B, const int m, const int n);

// Tested
void copy_vector(const float32_t* A, float32_t* B, const int n);

// Tested
void transpose_matrix(const float32_t* A, float32_t* B, const int m, const int n);

// Tested
void one_hot_matrix(const int* A, float32_t* B, const int m, const int n);

// Tested
void multiply_matrix_scalar(const float32_t* A, float32_t* B, const float32_t x,
                                  const int m, const int n);

// Tested
void multiply_matrix_matrix(const float32_t* A, const float32_t* B, float32_t* C,
                                  const int m, const int l, const int n);
// Tested
void multiply_matrix_hadamard(const float32_t* A, const float32_t* B, float32_t* C,
                                    const int m, const int n);
// Tested
void compare_vector(const int* A, const int* B, int* compare, const int n);

// Tested
void matrix_redux_float(const float32_t* A, float32_t* B, const int m, const int n);

// Tested
void matrix_redux_int(const int* A, int* B, const int m, const int n);

#endif  // MATRIXNEON_H
