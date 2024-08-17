#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "neural_network.h"
#include "server_fsm.h"
#include "socket_wrapper.h"
#include "time.h"

#define MATRIX_M 10
#define MATRIX_N 7
#define MATRIX_O 9

int main_server();
int main_test();

int main() {
  printf("Started\n");
  return main_server();
}

int main_server() {  // CNN Variables
  float *data_set;
  neural_network_t cnn;
  int *expected_output, *result, accuracy;
  int data_set_size, iterations;

  // FSM Variables
  int state, sock, mode, action_result, server_fd;

  state = INIT;

  printf("Entering state machine\n");
  while (1) {
    action_result = action(state, sock, result, data_set_size, &data_set);
    switch (state) {
      case INIT:
        server_fd = action_result;
        result = &server_fd;
        break;
      case LISTEN:
        sock = action_result;
        break;
      case IDLE:
        mode = action_result;
        break;
      case WAIT_TRAIN_DATA:
        data_set_size = action_result;
        result = malloc((sizeof(int) * data_set_size));
        break;
      case WAIT_TRAIN_LABELS:
        expected_output = result;
        break;
      case WAIT_TRAIN_ITERATIONS:
        struct timespec inicio, fim;
        long segundos, nanosegundos;
        double tempo_gasto;
        // Marca o tempo de início
        iterations = action_result;
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        accuracy =
            train(data_set, expected_output, &cnn, data_set_size, iterations);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        segundos = fim.tv_sec - inicio.tv_sec;
        nanosegundos = fim.tv_nsec - inicio.tv_nsec;
        tempo_gasto = segundos + nanosegundos * 1e-9;
        // Exibe o tempo gasto com precisão de milissegundos
        printf("Tempo de execução: %f segundos\n", tempo_gasto);
        result = &accuracy;
        free(expected_output);
        free(data_set);
        break;
      case SEND_TRAINING_RESULT:
        break;
      case WAIT_INFERENCE_DATA:
        data_set_size = action_result;
        result = inference(data_set, &cnn, data_set_size);
        free(data_set);
        break;
      case SEND_INFERENCE_RESULT:
        free(result);
        break;
      case END_CONNECTION:
        result = &server_fd;
        socket_close(server_fd);
        return 0;
        break;
      default:
    }
    state = next_state(state, mode);
  }

  return 0;
}

int main_test() {
  float *A = malloc(sizeof(float) * MATRIX_M * MATRIX_N);
  float *altA = malloc(sizeof(float) * MATRIX_M * MATRIX_N);
  float *B = malloc(sizeof(float) * MATRIX_M * MATRIX_N);
  float *D = malloc(sizeof(float) * MATRIX_O * MATRIX_N);
  float *fvec = malloc(sizeof(float) * MATRIX_M);

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
    fvec[i] = (MATRIX_M * 1.0 * i) / (MATRIX_N);

    int x = MATRIX_M - i;
    intA[i] = (x * x) % 10;
    intB[i] = x * x * x % 10;
  }

  for (i = 0; i < MATRIX_O; i++) {
    for (j = 0; j < MATRIX_N; j++) {
      D[MATRIX_N * i + j] = 17.0 / (i + j + 1);
    }
  }

  printf("\033[36mMatrizes operando:\n");
  // print_matrix(A, MATRIX_M, MATRIX_N);
  print_matrix(B, MATRIX_M, MATRIX_N);
  // print_matrix(fvec, MATRIX_M, 1);
  // print_matrix(D, MATRIX_O, MATRIX_N);
  // print_int_matrix(intA, MATRIX_M, 1);
  // print_int_matrix(intB, MATRIX_M, 1);
  // print_int_matrix(intMA, MATRIX_M, MATRIX_N);
  printf("\033[0m\n");

  // float * scalarA = multiply_matrix_scalar(A, -2.85, MATRIX_M, MATRIX_N);
  // float * reluA = relu_matrix(A, MATRIX_M, MATRIX_N);
  // float * C = sum_matrix(A, B, MATRIX_M, MATRIX_N);
  // float *mult = multiply_matrix_matrix(A, D, MATRIX_M, MATRIX_N, MATRIX_O);
  // float * diffAB = diff_matrix(A, B, MATRIX_M, MATRIX_N);
  // float * reluDevA = relu_derivate_matrix(A, MATRIX_M, MATRIX_N);
  // float * oneHotA = one_hot_matrix(intA, 10, MATRIX_M);
  // float *transposeB = transpose_matrix(B, MATRIX_M, MATRIX_N);
  // int * compareAB = compare_vector(intA, intB, MATRIX_M);

  // sum_multiply_matrix_scalar_fast(altA, B, 5.7, MATRIX_M, MATRIX_N);

  // float *minMaxA = minmax_matrix(A, MATRIX_M, MATRIX_N);
  // float * reduxB = matrix_redux_float(B, MATRIX_M, MATRIX_N);
  // int * reduxIntMA = matrix_redux_int(intMA, MATRIX_M, MATRIX_N);
  // float *sumAvec = sum_matrix_vector(A, fvec, MATRIX_M, MATRIX_N);
  // float *hadamard = multiply_matrix_hadamard(A, B, MATRIX_M, MATRIX_N);

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
  // print_matrix(transposeB, MATRIX_N, MATRIX_M);
  //  printf("\033[33mMinMax A\n");
  //  print_matrix(minMaxA, MATRIX_M, MATRIX_N);
  //  printf("\033[34mA x D\n");
  //  print_matrix(mult, MATRIX_M, MATRIX_O);
  //  printf("\033[35mCompare intA intB\n");
  //  print_int_matrix(compareAB, 1, MATRIX_M);
  //  printf("\033[31mredux B\n");
  //  print_matrix(reduxB, MATRIX_M, 1);
  //  printf("\033[33mredux intMA\n");
  //  print_int_matrix(reduxIntMA, MATRIX_M, 1);
  //  printf("\033[34mSum A fvec\n");
  //  print_matrix(sumAvec, MATRIX_M, MATRIX_N);
  //  printf("\033[35mHadamard A B\n");
  //  print_matrix(hadamard, MATRIX_M, MATRIX_N);
  printf("\033[0m");

  free(A);
  free(B);
  free(fvec);
  free(altA);
  free(D);
  free(intA);
  free(intB);
  free(intMA);

  // free(minMaxA);
  // free(sumAvec);
  // free(hadamard);
  // free(transposeB);
  return 0;
}
