
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

float32_t* transpose_matrix(float32_t* A, int m, int n) {
  float32_t* B = malloc(sizeof(float32_t) * n * m);
  int m_iter = m - m % 4;
  int n_iter = n - n % 4;
  int i, j, j2;
  for (i = 0; i < m_iter; i += 4) {
    for (j = 0; j < n_iter; j += 4) {
      float32x4_t a1 = vld1q_f32(A + n * i + j);
      float32x4_t a2 = vld1q_f32(A + n * (i + 1) + j);
      float32x4_t a3 = vld1q_f32(A + n * (i + 2) + j);
      float32x4_t a4 = vld1q_f32(A + n * (i + 3) + j);
      float32x4x2_t zip1 = vzipq_f32(a1, a2);
      float32x4x2_t zip2 = vzipq_f32(a3, a4);
      float32x4_t b1 =
          vcombine_f32(vget_low_f32(zip1.val[0]), vget_low_f32(zip2.val[0]));
      float32x4_t b2 =
          vcombine_f32(vget_high_f32(zip1.val[0]), vget_high_f32(zip2.val[0]));
      float32x4_t b3 =
          vcombine_f32(vget_low_f32(zip1.val[1]), vget_low_f32(zip2.val[1]));
      float32x4_t b4 =
          vcombine_f32(vget_high_f32(zip1.val[1]), vget_high_f32(zip2.val[1]));
      vst1q_f32(B + n * j + i, b1);
      vst1q_f32(B + n * (j + 1) + i, b2);
      vst1q_f32(B + n * (j + 2) + i, b3);
      vst1q_f32(B + n * (j + 3) + i, b4);
    }

    if (j < n) {
      float32x4_t a1 = vld1q_f32(A + n * i + j);
      float32x4_t a2 = vld1q_f32(A + n * (i + 1) + j);
      float32x4_t a3 = vld1q_f32(A + n * (i + 2) + j);
      float32x4_t a4 = vld1q_f32(A + n * (i + 3) + j);
      float32x4x2_t zip1 = vzipq_f32(a1, a2);
      float32x4x2_t zip2 = vzipq_f32(a3, a4);
      float32x4_t b1 =
          vcombine_f32(vget_low_f32(zip1.val[0]), vget_low_f32(zip2.val[0]));
      float32x4_t b2 =
          vcombine_f32(vget_high_f32(zip1.val[0]), vget_high_f32(zip2.val[0]));
      float32x4_t b3 =
          vcombine_f32(vget_low_f32(zip1.val[1]), vget_low_f32(zip2.val[1]));
      float32x4_t b4 =
          vcombine_f32(vget_high_f32(zip1.val[1]), vget_high_f32(zip2.val[1]));
      j2 = j;
      while (j2 < n) {
        vst1q_f32(B + n * j2 + i, b1);
        b1 = b2;
        b2 = b3;
        b3 = b4;
        j2++;
      }
    }
  }
  while (i < m) {
    for (j = 0; j < n; j++) {
      B[j * n + i] = A[i * n + j];
    }
    i++;
  }
  return B;
}
