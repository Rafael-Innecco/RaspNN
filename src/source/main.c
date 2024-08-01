#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void print_matrix(float32_t*, int, int);

// Placeholder
int main() {

  printf("Starting\n");

  float32_t * A = malloc(sizeof(float32_t) * 20);
  float32_t * B = malloc(sizeof(float32_t) * 20);
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      if (j % 2 == 0) {
        A[5*i + j] = - 4.0 / (i + j + 1.0);
      } else {
        A[5*i + j] = 4.0 / (i + j + 1.0);
      }
      B[5*i + j] = 15.0 / (i + j + 2.0);
    }
  }

  print_matrix(A, 4, 5);
  print_matrix(B, 4, 5);

  float32_t * halfA = multiply_matrix_scalar(A, -2.85, 4, 5);
  float32_t * reluA = relu_matrix(A, 4, 5);
  float32_t * C = sum_matrix(A, B, 4, 5);

  print_matrix(halfA, 4, 5);
  print_matrix(reluA, 4, 5);
  print_matrix(C, 4, 5);

  return 0;
}

void print_matrix(float32_t* A, int m, int n) {
  int i, j;
  printf("\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf(" %f ", A[n*i + j]);
    }
    printf("\n");
  }
  printf("\n");
}
