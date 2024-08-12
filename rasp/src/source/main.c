#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void print_matrix(float32_t*, int, int);
void print_int_matrix(int*, int, int);

// Placeholder
int main() {

  float32_t * A = malloc(sizeof(float32_t) * 30);
  float32_t * altA = malloc(sizeof(float32_t) * 30);
  float32_t * i5 = malloc(sizeof(float32_t) * 25);
  float32_t * B = malloc(sizeof(float32_t) * 30);

  int * intA = malloc (sizeof(float32_t) * 5);

  int i, j;

  for (i = 0; i < 5; i++) {
    for (j = 0; j < 6; j++) {
      if (j % 2 == 0) {
         A[6*i + j] = - 4.0 / (i + j + 1.0);
      } else {
        A[6*i + j] = 4.0 / (i + j + 1.0);
      }
      if (i == j && i < 5 && j < 5) {
        i5[6*i + j] = 1;
      } else {
        i5[6*i + j] = 0;
      }
      B[6*i + j] = 10*i;
      altA[6*i + j] = A[6*i + j];
    }
      
    intA[i] = i*i % 10;
  }

  printf("\033[36mMatrizes operando:\n");
  print_matrix(A, 5, 6);
  print_matrix(B, 5, 6);
  print_int_matrix(intA, 5, 1);
  printf("\033[0m\n");

  float32_t * scalarA = multiply_matrix_scalar(A, -2.85, 5, 6);
  float32_t * reluA = relu_matrix(A, 5, 6);
  float32_t * C = sum_matrix(A, B, 5, 6);
  float32_t * mult = multiply_matrix_matrix(A, B, 5, 6, 5);
  float32_t * diffAB = diff_matrix(A, B, 5, 6);
  float32_t * reluDevA = relu_derivate_matrix(A, 5, 6);
  float32_t * oneHotA = one_hot_matrix(intA, 10, 5);
  float32_t * transposeB = transpose_matrix(B, 5 , 6);

  sum_multiply_matrix_scalar_fast(altA, B, 5.7, 5, 6);

  // float32_t * minMaxA = minmax_matrix(A, 5, 6);

  // printf("\033[33mScalar multiplication A\n");
  // print_matrix(scalarA, 5, 6);
  // printf("\033[34mReLU A\n");
  // print_matrix(reluA, 5, 6);
  // printf("\033[35mSum A + B\n");
  // print_matrix(C, 5, 6);
  // printf("\033[31mdiff A - B\n");
  // print_matrix(diffAB, 5, 6);
  // printf("\033[33mA + x * B\n");
  // print_matrix(altA, 5, 6);
  // printf("\033[34mReLU derivate A\n");
  // print_matrix(reluDevA, 5, 6);
  // printf("\033[35moneHot A\n");
  // print_matrix(oneHotA, 10, 5);
  printf("\033[31mTraspose B\n");
  print_matrix(transposeB, 6, 5);

  printf("\033[0m");
 return 0;
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
