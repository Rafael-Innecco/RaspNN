
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "math_func.h"

float32_t* init_matrix(const float32_t x, const int m, const int n) {
  float32_t* A = malloc(sizeof(float32_t) * n * m);
  int i;
  int iter = (n * m) - ((n * m) % 4);
  float32x4_t init_value = vmovq_n_f32(x);
  for (i = 0; i < iter; i += 4) {
    vst1q_f32(A + i, init_value);
  }

  while (i < n * m) {
    A[i] = 0.0;
    i++;
  }
  return A;
}

float32_t* init_matrix_random(const int m, const int n) {
  float32_t* A = malloc(sizeof(float32_t) * n * m);

  srand(time(NULL));  // Semente aleatoria

  // Nada a ser paralelizado
  for (int i = 0; i < n * m; i++) {
    A[i] = (float)rand() / (float)RAND_MAX;
  }
  return A;
}

float32_t* sum_matrix(const float32_t* A, const float32_t* B, const int m,
                      const int n) {
  int i;
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int iter = (n * m) - ((n * m) % 4);
  for (i = 0; i < iter; i += 4) {
    float32x4_t a = vld1q_f32(A + i);
    float32x4_t b = vld1q_f32(B + i);
    float32x4_t c = vaddq_f32(a, b);
    vst1q_f32(C + i, c);
  }

  while (i < n * m) {
    C[i] = A[i] + B[i];
    i++;
  }

  return C;
}

float32_t* sum_matrix_vector(const float32_t* A, const float32_t* B,
                             const int m, const int n) {
  int i, j;
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int n_iter = n - (n % 4);
  float32x4_t b;

  for (i = 0; i < m; i++) {
    b = vmoq_n_f32(B[i]);
    for (j = 0; j < n_iter; j += 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      float32x4_t c = vaddq_f32(a, b);
      vst1q_f32(C + n * i + j, c);
    }

    while (j < n) {
      C[n * i + j] = A[n + i + j] + B[i];
      j++;
    }
  }

  return C;
}

float32_t* diff_matrix(const float32_t* A, const float32_t* B, const int m,
                       const int n) {
  int i;
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int iter = (n * m) - ((n * m) % 4);
  for (i = 0; i < iter; i += 4) {
    float32x4_t a = vld1q_f32(A + i);
    float32x4_t b = vld1q_f32(B + i);
    float32x4_t c = vsubq_f32(a, b);
    vst1q_f32(C + i, c);
  }

  while (i < n * m) {
    C[i] = A[i] - B[i];
    i++;
  }
  return C;
}

float32_t* relu_matrix(const float32_t* A, const int m, const int n) {
  int i;
  float32_t* B = malloc(sizeof(float32_t) * m * n);
  int iter = (n * m) - ((n * m) % 4);
  for (i = 0; i < iter; i += 4) {
    float32x4_t b = relu(vld1q_f32(A + i));
    vst1q_f32(B + i, b);
  }

  while (i < n * m) {
    if (A[i] > 0) {
      B[i] = A[i];
    } else {
      B[i] = 0;
    }
    i++;
  }

  return B;
}

float32_t* relu_derivate_matrix(const float32_t* A, const int m, const int n) {
  int i;
  float32_t* B = malloc(sizeof(float32_t) * m * n);
  int iter = (n * m) - ((n * m) % 4);
  for (i = 0; i < iter; i += 4) {
    float32x4_t b = relu_derivate(vld1q_f32(A + i));
    vst1q_f32(B + i, b);
  }

  while (i < n * m) {
    if (A[i] > 0) {
      B[i] = 1;
    } else {
      B[i] = 0;
    }
    i++;
  }

  return B;
}

float32_t* multiply_matrix_scalar(const float32_t* A, const float32_t x,
                                  const int m, const int n) {
  float32_t* B = malloc(sizeof(float32_t) * n * m);
  int iter = (n * m) - ((n * m) % 4);
  int i;
  for (i = 0; i < iter; i += 4) {
    float32x4_t a = vld1q_f32(A + i);
    float32x4_t b = vmulq_n_f32(a, x);
    vst1q_f32(B + i, b);
  }

  while (i < n * m) {
    B[i] = x * A[i];
    i++;
  }
  return B;
}

void sum_multiply_matrix_scalar_fast(float32_t* A, const float32_t* B,
                                     const float32_t x, const int m,
                                     const int n) {
  int i;
  int iter = (n * m) - ((n * m) % 4);
  for (i = 0; i < iter; i += 4) {
    float32x4_t a = vld1q_f32(A + i);
    float32x4_t b = vld1q_f32(B + i);
    float32x4_t c = vmlaq_n_f32(a, b, x);
    vst1q_f32(A + i, c);
  }

  while (i < n * m) {
    A[i] = A[i] + x * B[i];
    i++;
  }
  return;
}

float32_t max_vector_fast(float32_t* A, const int n) {
  float32_t max = -1.0 * FLOAT_MAX;
  int m = (n % 8) ? ((n >> 3) + 1) : (n >> 3);  // m = ceil(n/8)
  int n_iter = n - n % 8;
  int i;
  for (i = 0; i < n_iter; i += 8) {
    float32x4_t a_max = vld1q_f32(A + i);
    float32x4_t b_max = vld1q_f32(A + i + 4);
    float32x4_t max_vector = vmaxq_f32(a_max, b_max);
    vst1q_f32(A + m + (i >> 1), max_vector);
  }
  if (n % 8) {
    while (i < n) {
      if (A[i] > max) max = A[i];
      i++;
    }
    A[m - 1] = max;
    if (n < 8) return max;
  }
  return max_vector_fast(A, m);
}

float32_t min_vector_fast(float32_t* A, const int n) {
  float32_t min = FLOAT_MAX;
  int m = (n % 8) ? ((n >> 3) + 1) : (n >> 3);  // m = ceil(n/8)
  int n_iter = n - n % 8;
  int i;
  for (i = 0; i < n_iter; i += 8) {
    float32x4_t a_min = vld1q_f32(A + i);
    float32x4_t b_min = vld1q_f32(A + i + 4);
    float32x4_t min_vector = vminq_f32(a_min, b_min);
    vst1q_f32(A + (i >> 1), min_vector);
  }
  if (n % 8) {
    while (i < n) {
      if (A[i] < min) min = A[i];
      i++;
    }
    A[m - 1] = min;
    if (n < 8) return min;
  }
  return min_vector_fast(A, m);
}

float32_t* minmax_matrix(const float32_t* A, const int m, const int n) {
  float32_t* B = malloc(sizeof(float32_t) * n * m);
  float32_t* A_T = transpose_matrix(A, m, n);
  float32_t* vec = malloc(sizeof(float32_t) * m);
  float32_t min, max;
  int i, j;
  int m_iter = m - m % 4;
  for (i = 0; i < n; i++) {
    copy_vector(A_T + m * i, vec, m);
    min = min_vector_fast(vec, m);
    copy_vector(A_T + m * i, vec, m);
    max = max_vector_fast(vec, m);
    float32_t inverse_interval = 1 / (max - min);
    float32_t min_normalized = -min / (max - min);
    float32x4_t min_normalized_vec = vmovq_n_f32(min_normalized);
    for (j = 0; j < m_iter; j += 4) {
      float32x4_t a = vld1q_f32(A_T + m * i + j);
      float32x4_t result = vmlaq_n_f32(min_normalized_vec, a, inverse_interval);
      vst1q_f32(B + m * i + j, result);
    }
    while (j < m) {
      B[m * i + j] = A_T[m * i + j] * inverse_interval + min_normalized;
      j++;
    }
  }
  float32_t* C = transpose_matrix(B, n, m);
  free(B);
  free(vec);
  free(A_T);
  return C;
}

void copy_vector(const float32_t* A, float32_t* B, const int n) {
  int i = 0;
  int n_iter = n - n % 4;
  // for (i = 0; i < n_iter; i += 4) {
  //   float32x4_t a = vld1q_f32(A + i);
  //   vst1q_f32(B + i, a);
  // }
  while (i < n) {
    B[i] = A[i];
    i++;
  }
}

float32_t* one_hot_matrix(const int* A, const int m, const int n) {
  float32_t* B = init_matrix(0.0, m, n);
  // Nada a ser paralelizado
  for (int i = 0; i < n; i++) {
    B[n * A[i] + i] = 1.0;
  }
}

float32_t* transpose_matrix(const float32_t* A, const int m, const int n) {
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
      vst1q_f32(B + m * j + i, b1);
      vst1q_f32(B + m * (j + 1) + i, b2);
      vst1q_f32(B + m * (j + 2) + i, b3);
      vst1q_f32(B + m * (j + 3) + i, b4);
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
        vst1q_f32(B + m * j2 + i, b1);
        b1 = b2;
        b2 = b3;
        b3 = b4;
        j2++;
      }
    }
  }

  while (i < m) {
    for (j = 0; j < n; j++) {
      B[j * m + i] = A[i * n + j];
    }
    i++;
  }
  return B;
}

// Não multiplica linha X coluna, e sim linha X linha (transposta)
// As dimensões devem ser mxl e nxl!!!!
float32_t* multiply_matrix_matrix(const float32_t* A, const float32_t* B,
                                  const int m, const int l, const int n) {
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int l_iter = l - l % 4;
  int i, j, k;
  float32x4_t sum;
  float32_t result;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      sum = vmovq_n_f32(0);
      result = 0.0;
      for (k = 0; k < l_iter; k += 4) {
        float32x4_t a = vld1q_f32(A + l * i + k);
        float32x4_t b = vld1q_f32(B + l * j + k);
        sum = vfmaq_f32(sum, a, b);
      }

      while (k < l) {
        result += A[l * i + k] * B[l * j + k];
        k++;
      }

      result += vgetq_lane_f32(sum, 0);
      result += vgetq_lane_f32(sum, 1);
      result += vgetq_lane_f32(sum, 2);
      result += vgetq_lane_f32(sum, 3);

      C[n * i + j] = result;
    }
  }
  return C;
}

float32_t* multiply_matrix_hadamard(const float32_t* A, const float32_t* B,
                                    const int m, const int n) {
  int i;
  float32_t* C = malloc(sizeof(float32_t) * n * m);
  int iter = (n * m) - ((n * m) % 4);
  for (i = 0; i < iter; i += 4) {
    float32x4_t a = vld1q_f32(A + i);
    float32x4_t b = vld1q_f32(B + i);
    float32x4_t c = vmulq_f32(a, b);
    vst1q_f32(C + i, c);
  }

  while (i < n * m) {
    C[i] = A[i] * B[i];
    i++;
  }

  return C;
}

int* compare_vector(const int* A, const int* B, const int n) {
  int* compare = malloc(sizeof(int) * n);
  int i, j;
  int n_iter = n - n % 4;
  int32x4_t zeros = vmovq_n_s32(0);
  int32x4_t ones = vmovq_n_s32(1);
  for (j = 0; j < n_iter; j += 4) {
    int32x4_t a = vld1q_s32(A + i);
    int32x4_t b = vld1q_s32(B + i);
    // Checar o compilado
    int32x4_t equal = vbslq_s32(vceqq_s32(a, b), ones, zeros);
    vst1q_s32(compare + j, equal);
  }

  while (j < n) {
    compare[j] = (A[j] == B[j]);
    j++;
  }

  return compare;
}

float32_t* matrix_redux_float(const float32_t* A, const int m, const int n) {
  int i, j;
  float32_t* B = malloc(sizeof(float32_t) * m);
  float32x4_t sum;
  float32_t result;
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    sum = vmovq_n_f32(0.0);
    result = 0.0;
    for (j = 0; j < n_iter; j += 4) {
      float32x4_t a = vld1q_f32(A + n * i + j);
      sum = vaddq_f32(a, sum);
    }

    while (j < n) {
      result += A[n * i + j];
      j++;
    }

    result += vgetq_lane_f32(sum, 0);
    result += vgetq_lane_f32(sum, 1);
    result += vgetq_lane_f32(sum, 2);
    result += vgetq_lane_f32(sum, 3);

    B[i] = result;
  }
  return B;
}

int* matrix_redux_int(const int* A, const int m, const int n) {
  int i, j;
  int* B = malloc(sizeof(int) * m);
  int32x4_t sum;
  int result;
  int n_iter = n - n % 4;
  for (i = 0; i < m; i++) {
    sum = vmovq_n_s32(0);
    result = 0.0;
    for (j = 0; j < n_iter; j += 4) {
      int32x4_t a = vld1q_s32(A + n * i + j);
      sum = vaddq_s32(a, sum);
    }

    while (j < n) {
      result += A[n * i + j];
      j++;
    }

    result += vgetq_lane_s32(sum, 0);
    result += vgetq_lane_s32(sum, 1);
    result += vgetq_lane_s32(sum, 2);
    result += vgetq_lane_s32(sum, 3);

    B[i] = result;
  }
  return B;
}

void print_matrix(float32_t* A, int m, int n) {
  int i, j;
  printf("\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("\t%f ", A[n*i + j]);
    }
    printf("\n");
  }
  printf("\n");
}


void print_int_matrix(int* A, int m, int n) {
  int i, j;
  printf("\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("\t%d ", A[n*i + j]);
    }
    printf("\n");
  }
  printf("\n");
}


