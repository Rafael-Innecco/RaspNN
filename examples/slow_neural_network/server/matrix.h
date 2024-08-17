#ifndef MATRIXNEON_H
#define MATRIXNEON_H

#define FLOAT_MAX 0X7F800000

// Some utility funcitons

void init_matrix(float* A, const float x, const int m, const int n);

void init_matrix_random(float* A, const int m, const int n);

void print_matrix(const float*, int, int);

void print_int_matrix(const int*, int, int);

void print_matrix_parcial(const float* A, int m, int n, int k);

// Matrix operations

// Tested
void sum_matrix(const float* A, const float* B, float* C, const int m,
                const int n);

// Tested
void sum_matrix_vector(const float* A, const float* B, float* C, const int m,
                       const int n);

// Tested
void diff_matrix(const float* A, const float* B, float* C, const int m,
                 const int n);

// Tested
void sum_multiply_matrix_scalar_fast(float* A, const float* B, const float x,
                                     const int m, const int n);
// Tested
void relu_matrix(const float* A, float* B, const int m, const int n);

// Tested
void relu_derivate_matrix(const float* A, float* B, const int m, const int n);

// Tested
float max_vector_fast(float* A, const int n);

// Tested
float min_vector_fast(float* A, const int n);

// Tested
void minmax_matrix(const float* A, float* B, const int m, const int n);

void softmax_matrix(const float* A, float* B, const int m, const int n);

// Tested
void copy_vector(const float* A, float* B, const int n);

// Tested
void transpose_matrix(const float* A, float* B, const int m, const int n);

// Tested
void one_hot_matrix(const int* A, float* B, const int m, const int n);

// Tested
void multiply_matrix_scalar(const float* A, float* B, const float x,
                            const int m, const int n);

void multiply_matrix_scalar_fast(float* A, const float x, const int m,
                                 const int n);

// Tested
void multiply_matrix_matrix(const float* A, const float* B, float* C,
                            const int m, const int l, const int n);

// Tested
void multiply_matrix_hadamard(const float* A, const float* B, float* C,
                              const int m, const int n);

// Tested
void compare_vector(const int* A, const int* B, int* compare, const int n);

// Tested
void matrix_redux_float(const float* A, float* B, const int m, const int n);

// Tested
void matrix_redux_int(const int* A, int* B, const int m, const int n);

#endif  // MATRIXNEON_H
