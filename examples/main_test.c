#include <arm_neon.h>

int main() {
  float32_t *A = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *altA = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *B = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *D = malloc(sizeof(float32_t) * MATRIX_O * MATRIX_N);
  float32_t *fvec = malloc(sizeof(float32_t) * MATRIX_N);

  int *intA = malloc(sizeof(int) * MATRIX_M);
  int *intB = malloc(sizeof(int) * MATRIX_M);
  int *intMA = malloc(sizeof(int) * MATRIX_M * MATRIX_N);

  int i, j;

  for (i = 0; i < MATRIX_M; i++) {
    for (j = 0; j < MATRIX_N; j++) {
      if (j % 2 == 0) {
        A[MATRIX_N * i + j] = -4.0 / (i + j + 1.0);
      } else {
        A[MATRIX_N * i + j] = 4.0 / (i + j + 1.0);
      }
      B[MATRIX_N * i + j] = 10 * (i + 1) + j + 1;
      altA[MATRIX_N * i + j] = A[MATRIX_N * i + j];

      intMA[MATRIX_N * i + j] = 13 * (i + 1) * (j + 1) % 17;
    }

    int x = MATRIX_M - i;
    intA[i] = (x * x) % 10;
    intB[i] = x * x * x % 10;
  }

  for (i = 0; i < MATRIX_O; i++) {
    for (j = 0; j < MATRIX_N; j++) {
      D[MATRIX_N * i + j] = 17 % (i + j);
    }
  }

  for (i = 0; i < MATRIX_N; i++) {
    fvec[i] = (MATRIX_M * 1.0 * i) / (MATRIX_N);
  }

  printf("\033[36mMatrizes operando:\n");
  print_matrix(A, MATRIX_M, MATRIX_N);
  // print_matrix(B, MATRIX_M, MATRIX_N);
  print_matrix(fvec, 1, MATRIX_N);
  // print_matrix(D, MATRIX_O, MATRIX_N);
  // print_int_matrix(intA, 1, MATRIX_M);
  // print_int_matrix(intB, 1, MATRIX_M);
  // print_int_matrix(intMA, MATRIX_M, MATRIX_N);
  printf("\033[0m\n");

  float32_t *result = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *resultMult = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_O);
  float32_t *resultReduxB = malloc(sizeof(float32_t) * MATRIX_M);
  float32_t *oneHotA = malloc(sizeof(float32_t) * 10 * MATRIX_M);
  int *resultIntVec = malloc(sizeof(int) * MATRIX_M);

  // multiply_matrix_scalar(A, result, -2.85, MATRIX_M, MATRIX_N);
  // printf("\033[33mScalar multiplication A\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);

  // relu_matrix(A, result, MATRIX_M, MATRIX_N);
  // printf("\033[34mReLU A\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);
  //
  // sum_matrix(A, B, result, MATRIX_M, MATRIX_N);
  // printf("\033[35mSum A + B\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);

  // diff_matrix(A, B, result, MATRIX_M, MATRIX_N);
  // printf("\033[31mdiff A - B\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);

  // relu_derivate_matrix(A, result, MATRIX_M, MATRIX_N);
  // printf("\033[34mReLU derivate A\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);
  //
  // sum_multiply_matrix_scalar_fast(altA, B, 5.7, MATRIX_M, MATRIX_N);
  //  printf("\033[33mA + x * B\n");
  //  print_matrix(altA, MATRIX_M, MATRIX_N);

  // transpose_matrix(B, result, MATRIX_M, MATRIX_N);
  // printf("\033[31mTraspose B\n");
  // print_matrix(result, MATRIX_N, MATRIX_M);

  // minmax_matrix(A, result, MATRIX_M, MATRIX_N);
  // printf("\033[33mMinMax A\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);

  // multiply_matrix_matrix(A, D, resultMult, MATRIX_M, MATRIX_N, MATRIX_O);
  // printf("\033[34mA x D\n");
  // print_matrix(resultMult, MATRIX_M, MATRIX_O);

  // matrix_redux_float(B, resultReduxB, MATRIX_M, MATRIX_N);
  // printf("\033[31mredux B\n");
  // print_matrix(resultReduxB, MATRIX_M, 1);

  // one_hot_matrix(intA, oneHotA, MATRIX_M, 10);
  // printf("\033[35moneHot A\n");
  // print_matrix(oneHotA, MATRIX_M, 10);

  // multiply_matrix_hadamard(A, B, result, MATRIX_M, MATRIX_N);
  // printf("\033[35mHadamard A B\n");
  // print_matrix(result, MATRIX_M, MATRIX_N);

  // compare_vector(intA, intB, resultIntVec,  MATRIX_M);
  // printf("\033[35mCompare intA intB\n");
  // print_int_matrix(resultIntVec, 1, MATRIX_M);

  // matrix_redux_int(intMA, resultIntVec, MATRIX_M, MATRIX_N);
  // printf("\033[33mredux intMA\n");
  // print_int_matrix(resultIntVec, MATRIX_M, 1);

  sum_matrix_vector(A, fvec, result, MATRIX_M, MATRIX_N);
  printf("\033[34mSum A fvec\n");
  print_matrix(result, MATRIX_M, MATRIX_N);

  printf("\033[0m");

  free(A);
  free(B);
  free(fvec);
  free(altA);
  free(D);
  free(intA);
  free(intB);
  free(intMA);

  free(result);
  free(resultMult);
  free(resultReduxB);
  free(oneHotA);
  free(resultIntVec);
  return 0;
}
