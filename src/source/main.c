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

  float16x8_t test_variable;
  float16_t teste_2;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      printf("\033[31m%d %d %f %f\033[0m\n", i, j, i + j + 1.0, i + j + 2.0);
      A[5*i + j] = 4.0 / (i + j + 1.0);
      B[5*i + j] = 15.0 / (i + j + 2.0);
    }
  }

  print_matrix(A, 4, 5);
  print_matrix(B, 4, 5);

  float32_t * C = sum_matrix(A, B, 4, 5);

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
