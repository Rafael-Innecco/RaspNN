
#include "matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_matrix(float* A, const float x, const int m, const int n) {
  for (int i = 0; i < n * m; i++) {
    A[i] = x;
  }
  return;
}

void init_matrix_random(float* A, const int m, const int n) {
  // srand(time(NULL));  // Semente aleatoria

  for (int i = 0; i < n * m; i++) {
    A[i] = (float)rand() / (float)RAND_MAX - 0.5;
  }
  return;
}

void sum_matrix(const float* A, const float* B, float* C, const int m,
                const int n) {
  for (int i = 0; i < n * m; i++) {
    C[i] = A[i] + B[i];
  }
  return;
}

/***
 * O vetor B deve ter dimensão N x 1
 ***/
void sum_matrix_vector(const float* A, const float* B, float* C, const int m,
                       const int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      C[n * i + j] = A[n * i + j] + B[j];
    }
  }

  return;
}

void diff_matrix(const float* A, const float* B, float* C, const int m,
                 const int n) {
  for (int i = 0; i < n * m; i++) {
    C[i] = A[i] - B[i];
  }
  return;
}

void relu_matrix(const float* A, float* B, const int m, const int n) {
  for (int i = 0; i < n * m; i++) {
    B[i] = (A[i] > 0) ? A[i] : 0;
  }
  return;
}

void relu_derivate_matrix(const float* A, float* B, const int m, const int n) {
  for (int i = 0; i < n * m; i++) {
    B[i] = (A[i] > 0) ? 1 : 0;
  }
  return;
}

void multiply_matrix_scalar(const float* A, float* B, const float x,
                            const int m, const int n) {
  for (int i = 0; i < n * m; i++) {
    B[i] = x * A[i];
  }
  return;
}

void multiply_matrix_scalar_fast(float* A, const float x, const int m,
                                 const int n) {
  for (int i = 0; i < n * m; i++) {
    A[i] = x * A[i];
  }
  return;
}

void sum_multiply_matrix_scalar_fast(float* A, const float* B, const float x,
                                     const int m, const int n) {
  for (int i = 0; i < n * m; i++) {
    A[i] += x * B[i];
  }
  return;
}

float max_vector_fast(float* A, const int n) {
  float max = -1.0 * FLOAT_MAX;
  for (int i = 0; i < n; i++) {
    if (A[i] > max) max = A[i];
  }
  return max;
}

float min_vector_fast(float* A, const int n) {
  float min = FLOAT_MAX;
  for (int i = 0; i < n; i++) {
    if (A[i] < min) min = A[i];
  }
  return min;
}

void minmax_matrix(const float* A, float* B, const int m, const int n) {
  float min, max;
  for (int j = 0; j < n; j++) {
    max = -1.0 * FLOAT_MAX;
    min = FLOAT_MAX;
    for (int i = 0; i < m; i++) {
      if (A[n * i + j] > max) max = A[n * i + j];
      if (A[n * i + j] < min) min = A[n * i + j];
    }
    for (int i = 0; i < m; i++) {
      B[n * i + j] = (A[n * i + j] - min) / (max - min);
    }
  }
  return;
}

void softmax_matrix(const float* A, float* B, const int m, const int n) {
  float sum;
  for (int i = 0; i < m; i++) {
    sum = 0.0;

    for (int j = 0; j < n; j++) {
      sum += exp(A[n * i + j]);
    }

    for (int j = 0; j < n; j++) {
      B[n * i + j] = exp(A[n * i + j]) / sum;
    }
  }
  return;
}

void copy_vector(const float* A, float* B, const int n) {
  for (int i = 0; i < n; i++) {
    B[i] = A[i];
  }
}

void one_hot_matrix(const int* A, float* B, const int m, const int n) {
  init_matrix(B, 0.0, m, n);
  for (int i = 0; i < n; i++) {
    B[n * i + A[i]] = 1.0;
  }
  return;
}

void transpose_matrix(const float* A, float* B, const int m, const int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      B[j * m + i] = A[i * n + j];
    }
  }
  return;
}

// Não multiplica linha X coluna, e sim linha X linha (transposta)
// As dimensões devem ser mxl e nxl!!!!
void multiply_matrix_matrix(const float* A, const float* B, float* C,
                            const int m, const int l, const int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      C[n * i + j] = 0.0;
      for (int k = 0; k < l; k++) {
        C[n * i + j] += A[l * i + k] * B[l * j + k];
      }
    }
  }
  return;
}

void multiply_matrix_hadamard(const float* A, const float* B, float* C,
                              const int m, const int n) {
  for (int i = 0; i < n * m; i++) {
    C[i] = A[i] * B[i];
  }
  return;
}

void compare_vector(const int* A, const int* B, int* compare, const int n) {
  for (int i = 0; i < n; i++) {
    compare[i] = (A[i] == B[i]);
  }
  return;
}

void matrix_redux_float(const float* A, float* B, const int m, const int n) {
  for (int i = 0; i < m; i++) {
    B[i] = 0;
    for (int j = 0; j < n; j++) {
      B[i] += A[n * i + j];
    }
  }
  return;
}

void matrix_redux_int(const int* A, int* B, const int m, const int n) {
  for (int i = 0; i < m; i++) {
    B[i] = 0;
    for (int j = 0; j < n; j++) {
      B[i] += A[n * i + j];
    }
  }
  return;
}

void print_matrix(const float* A, int m, int n) {
  int i, j;
  printf("\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("\t%3.2f ", A[n * i + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void print_matrix_parcial(const float* A, int m, int n, int k) {
  int i, j;
  printf("\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < k; j++) {
      printf("\t%.4f ", A[n * i + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void print_int_matrix(const int* A, int m, int n) {
  int i, j;
  printf("\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("\t%d ", A[n * i + j]);
    }
    printf("\n");
  }
  printf("\n");
}
