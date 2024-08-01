
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "math_func.h"

float16_t* sum_matrix(float16_t * A, float16_t* B, const int m, const int n) {
  int i, j;
  float16_t * C = malloc(sizeof(float16_t)*n*m);
  int n_iter = n - n % 4;
  for(i = 0; i < m; i++) {
    for(j = 0; j < n_iter; j = j + 4) {
      float16x8_t a = vld1q_f32(A + n * i + j);
      float16x8_t b = vld1q_f32(B + n * i + j);
      float16x8_t c = vaddq_f32(a, b);
      vst1q_f32(C + n*i + j, c);
    }
    
    while (j < n) {
      C[n*i + j] = A[n*i + j] + B[n*i + j];
      j++;
    }
  }
  return C;
}

float16x8_t** relu_matrix(const float16x8_t** A, const int m, const int n) {
  float16x8_t** B = malloc(sizeof(float16x8_t)*n*m);
  for(int i = 0; i < m; i ++)
    for(int j = 0; j < n; j++)
      B[i][j] = relu(A[i][j]);
  return B;
}

float16_t * multiply_matrix_scalar(const float16x8_t** A, const float16_t x, const int m, const int n) {
  float16_t * B = malloc(sizeof(float16x8_t)*n*m);
  for(int i = 0; i < m; i ++) {
    for(int j = 0; j < n; j = j + 4) {
      B[i][j] = vmulq_n_f32(A[i][j], x);
    }
  }
  return B;
}

float16x8_t** multiply_matrix_matrix(const float16x8_t** A, const float16x8_t** B, const int l,
                                     const int m, const int n) {
  float16x8_t** C = malloc(sizeof(float16x8_t)*n*m);
  for(int i = 0; i < l; i ++)
    for(int j = 0; j < m; j++) {
      C[i][j] = vmovq_n_f32(0.0);
      for(int k = 0; k < n; k++)
        C[i][j] = vfmaq_f32(C[i][j], A[i][k],B[k][j]);
    }
  return C;
}
