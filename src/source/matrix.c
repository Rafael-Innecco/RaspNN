
#include <stdlib.h>
#include "matrix.h"
#include "math_func.h"

float32_t** sum_matrix(const float32_t** A, const float32_t** B, const int m, const int n) {
  float32_t** C = malloc(sizeof(float32_t)*n*m);
  for(int i = 0; i < m; i ++)
    for(int j = 0; j < n; j++)
      C[i][j] = vaddq_f32(A[i][j], B[i][j]);
}

float32_t** relu_matrix(const float32_t** A, const int m, const int n) {
  float32_t** B = malloc(sizeof(float32_t)*n*m);
  for(int i = 0; i < m; i ++)
    for(int j = 0; j < n; j++)
      B[i][j] = relu(A[i][j]);
}

float32_t** multiply_matrix_scalar(const float32_t** A, const float32_t x, const int m, const int n) {
  float32_t** B = malloc(sizeof(float32_t)*n*m);
  for(int i = 0; i < m; i ++)
    for(int j = 0; j < n; j++)
      B[i][j] = vmulq_n_f32(A[i][j], x);
}

float32_t** multiply_matrix_matrix(const float32_t** A, const float32_t** B, const int l,
                                     const int m, const int n) {
  float32_t** C = malloc(sizeof(float32_t)*n*m);
  for(int i = 0; i < l; i ++)
    for(int j = 0; j < m; j++) {
      C[i][j] = vmovq_n_f32(0.0);
      for(int k = 0; k < n; k++)
        C[i][j] = vfmaq_f32(C[i][j], A[i][k],);
    }
}