
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

#include "math_func.h"

float32_t* sum_matrix(float32_t* A, float32_t* B, const int m, const int n) {
  int i, j;
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t b = vld1q_f32(B + n * i + j);
      float32x4_t c = vaddq_f32(a, b);
      vst1q_f32(C + n * i + j, c);
    }

    while (j < n) {
      C[n * i + j] = A[n * i + j] + B[n * i + j];
      j++;
    }
  }
  return C;
}

float32_t* diff_matrix(float32_t* A, float32_t* B, const int m, const int n) {
  int i, j;
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t b = vld1q_f32(B + n * i + j);
      float32x4_t c = vsubq_f32(a, b);
      vst1q_f32(C + n * i + j, c);
    }

    while (j < n) {
      C[n * i + j] = A[n * i + j] - B[n * i + j];
      j++;
    }
  }
  return C;
}

float32_t* relu_matrix(const float32_t* A, const int m, const int n) {
  int i, j;
  float32_t* B = malloc(sizeof(float32_t) * m * n);
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t b = relu(vld1q_f32(A + n * i + j));
      vst1q_f32(B + n * i + j, b);
    }

    while (j < n) {
      if (A[n * i + j] > 0) {
        B[n * i + j] = A[n * i + j];
      } else {
        B[n * i + j] = 0;
      }
      j++;
    }
  }

  return B;
}

float32_t* relu_derivate_matrix(const float32_t* A, const int m, const int n) {
  int i, j;
  float32_t* B = malloc(sizeof(float32_t) * m * n);
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t b = relu_derivate(vld1q_f32(A + n * i + j));
      vst1q_f32(B + n * i + j, b);
    }

    while (j < n) {
      if (A[n * i + j] > 0) {
        B[n * i + j] = A[n * i + j];
      } else {
        B[n * i + j] = 0;
      }
      j++;
    }
  }

  return B;
}

float32_t* multiply_matrix_scalar(const float32_t* A, const float32_t x,
                                  const int m, const int n) {
  float32_t* B = malloc(sizeof(float32_t) * n * m);
  int n_iter = n - n % 4;
  int i, j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t b = vmulq_n_f32(a, x);
      vst1q_f32(B + n * i + j, b);
    }

    while (j < n) {
      B[n * i + j] = x * A[n * i + j];
      j++;
    }
  }
  return B;
}

void sum_multiply_matrix_scalar_fast(float32_t* A, const float32_t* B,
                                     const float32_t x, const int m,
                                     const int n) {
  int i, j;
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n_iter; j = j + 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t b = vld1q_f32(B + n * i + j);
      float32x4_t c = vmlaq_n_f32(a, b, x);
      vst1q_f32(A + n * i + j, c);
    }

    while (j < n) {
      A[n * i + j] = A[n * i + j] + x * B[n * i + j];
      j++;
    }
  }
  return;
}

void minmax_vector(float32_t* operand, float32_t* destination, int n) {
  if (n < 4) {
    // Caso base
    float32_t min = FLOAT_MAX;
    float32_t max = FLOAT_MIN;
    int i;
    for (i = 0; i < n; i++) {
    }
  }
}

float32_t* minmax_matrix(float32_t* A, int n, int m) {
  float32_t* B = malloc(sizeof(float32_t) * n * m);
}
