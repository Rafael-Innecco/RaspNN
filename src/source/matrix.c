
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "math_func.h"

float32_t* sum_matrix(float32_t * A, float32_t* B, const int m, const int n) {
  int i, j;
  float32_t * C = malloc(sizeof(float32_t)*n*m);
  int n_iter = n - n % 4;
  for(i = 0; i < m; i++) {
    for(j = 0; j < n_iter; j = j + 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t b = vld1q_f32(B + n * i + j);
      float32x4_t c = vaddq_f32(a, b);
      vst1q_f32(C + n*i + j, c);
    }
    
    while (j < n) {
      C[n*i + j] = A[n*i + j] + B[n*i + j];
      j++;
    }
  }
  return C;
}

float32_t * relu_matrix(const float32_t * A, const int m, const int n) {
  int i, j;
  float32_t * B = malloc(sizeof(float32_t) * m * n);
  int n_iter = n - n % 4;
  for(i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t b = relu(vld1q_f32(A + n*i + j));
      vst1q_f32(B + n*i + j, b);
    }
      

    while (j < n) {
      if (A[n*i + j] > 0) {
        B[n*i + j] = A[n*i + j];
      } else {
        B[n*i + j] = 0;
      }
      j++;
    }

  }

  return B;
}

float32_t * multiply_matrix_scalar(const float32_t*  A, const float32_t x, const int m, const int n) {
  float32_t * B = malloc(sizeof(float32_t)*n*m);
  int n_iter = n - n % 4;
  int i, j;
  for(i = 0; i < m; i ++) {
    for(j = 0; j < n_iter; j = j + 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t b = vmulq_n_f32(a, x);
      vst1q_f32(B + n*i + j, b);
    }

    while (j < n) {
      B[n*i + j] = x * A[n*i + j];
      j++;
    }
  }
  return B;
}

float32_t * multiply_matrix_matrix(const float32_t * A, const float32_t * B, const int m,
                                     const int l, const int n) {
  //float32_t * C = malloc(sizeof(float32_t)*m*n);
  //for(int i = 0; i < l; i ++)
  //  for(int j = 0; j < m; j++) {
  //    C[i][j] = vmovq_n_f32(0.0);
  //    for(int k = 0; k < n; k++)
  //      C[i][j] = vfmaq_f32(C[i][j], A[i][k],B[k][j]);
  //  }
  return A;
}
