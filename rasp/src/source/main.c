#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

#define MATRIX_M 4
#define MATRIX_N 4

// Placeholder
int main() {

  float32_t * A = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t * altA = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t * B = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);

  int * intA = malloc (sizeof(float32_t) * 5);

  int i, j;

  for (i = 0; i < MATRIX_M; i++) {
    for (j = 0; j < MATRIX_N; j++) {
      if (j % 2 == 0) {
         A[MATRIX_N*i + j] = - 4.0 / (i + j + 1.0);
      } else {
        A[MATRIX_N*i + j] = 4.0 / (i + j + 1.0);
      }
      B[MATRIX_N*i + j] = 10* (i + 1) + j + 1;
      altA[MATRIX_N*i + j] = A[MATRIX_N*i + j];
    }
    
    intA[i] = i*i % 10;
  }

  printf("\033[36mMatrizes operando:\n");
  print_matrix(A, MATRIX_M, MATRIX_N);
  print_matrix(B, MATRIX_M, MATRIX_N);
  // print_int_matrix(intA, 5, 1);
  printf("\033[0m\n");

  // float32_t * scalarA = multiply_matrix_scalar(A, -2.85, MATRIX_M, MATRIX_N);
  // float32_t * reluA = relu_matrix(A, MATRIX_M, MATRIX_N);
  // float32_t * C = sum_matrix(A, B, MATRIX_M, MATRIX_N);
  // float32_t * mult = multiply_matrix_matrix(A, B, MATRIX_M, MATRIX_N, MATRIX_M);
  // float32_t * diffAB = diff_matrix(A, B, MATRIX_M, MATRIX_N);
  // float32_t * reluDevA = relu_derivate_matrix(A, MATRIX_M, MATRIX_N);
  // float32_t * oneHotA = one_hot_matrix(intA, 10, 5);
  float32_t * transposeB = transpose_matrix(B, MATRIX_M, MATRIX_N);

  // sum_multiply_matrix_scalar_fast(altA, B, 5.7, MATRIX_M, MATRIX_N);

  float32_t * minMaxA = minmax_matrix(A, MATRIX_M, MATRIX_N);

  // printf("\033[33mScalar multiplication A\n");
  // print_matrix(scalarA, MATRIX_M, MATRIX_N);
  // printf("\033[34mReLU A\n");
  // print_matrix(reluA, MATRIX_M, MATRIX_N);
  // printf("\033[35mSum A + B\n");
  // print_matrix(C, MATRIX_M, MATRIX_N);
  // printf("\033[31mdiff A - B\n");
  // print_matrix(diffAB, MATRIX_M, MATRIX_N);
  // printf("\033[33mA + x * B\n");
  // print_matrix(altA, MATRIX_M, MATRIX_N);
  // printf("\033[34mReLU derivate A\n");
  // print_matrix(reluDevA, MATRIX_M, MATRIX_N);
  // printf("\033[35moneHot A\n");
  // print_matrix(oneHotA, 10, 5);
  printf("\033[31mTraspose B\n");
  print_matrix(transposeB, MATRIX_N, MATRIX_M);
  printf("\033[33mMinMax A\n");
  print_matrix(minMaxA, MATRIX_M, MATRIX_N);
  printf("\033[0m");


  free(A);
  free(B);
  free(altA);
  free(transposeB);
  free(intA);
  free(minMaxA);
  return 0;
}
