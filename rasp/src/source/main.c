#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "neural_network.h"
#include "server_fsm.h"
#include "socket_wrapper.h"

#define MATRIX_M 11
#define MATRIX_N 13
#define MATRIX_O 9

int main_server();
int main_test();

int main() {
  printf("Started\n");
  return main_server();
}

int main_server() {  // CNN Variables
  float32_t *data_set;
  float32_t *input_layer_weight, *input_layer_bias;
  float32_t *hidden_layer1_weight, *hidden_layer1_bias;
  float32_t *hidden_layer2_weight, *hidden_layer2_bias;
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
        printf("\033[36mDataset size = %d\n", data_set_size);
        break;
      case WAIT_TRAIN_LABELS:
        expected_output = result;
        break;
      case WAIT_TRAIN_ITERATIONS:
        iterations = action_result;
        accuracy = train(data_set, expected_output, &input_layer_weight,
                         &input_layer_bias, &hidden_layer1_weight,
                         &hidden_layer1_bias, &hidden_layer2_weight,
                         &hidden_layer2_bias, data_set_size, iterations);
        result = &accuracy;
        free(expected_output);
        free(data_set);
        break;
      case SEND_TRAINING_RESULT:
        break;
      case WAIT_INFERENCE_DATA:
        data_set_size = action_result;
        result =
            inference(data_set, input_layer_weight, input_layer_bias,
                      hidden_layer1_weight, hidden_layer1_bias,
                      hidden_layer2_weight, hidden_layer2_bias, data_set_size);
        free(data_set);
        break;
      case SEND_INFERENCE_RESULT:
        free(result);
        break;
      case END_CONNECTION:
        result = &server_fd;
        break;
      default:
    }
    state = next_state(state, mode);
  }

  return 0;
}

int main_test() {
  float32_t *A = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *altA = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *B = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t *D = malloc(sizeof(float32_t) * MATRIX_O * MATRIX_N);
  float32_t *fvec = malloc(sizeof(float32_t) * MATRIX_M);

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
      D[MATRIX_N * i + j] = 17 % (i + j);
    }
  }

  printf("\033[36mMatrizes operando:\n");
  print_matrix(A, MATRIX_M, MATRIX_N);
  print_matrix(B, MATRIX_M, MATRIX_N);
  // print_matrix(fvec, MATRIX_M, 1);
  // print_matrix(D, MATRIX_O, MATRIX_N);
  // print_int_matrix(intA, MATRIX_M, 1);
  // print_int_matrix(intB, MATRIX_M, 1);
  // print_int_matrix(intMA, MATRIX_M, MATRIX_N);
  printf("\033[0m\n");

  float32_t* result = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t* resultMult = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_O);
  float32_t* resultReduxB = malloc(sizeof(float32_t) * MATRIX_M);
  float32_t* oneHotA = malloc(sizeof(float32_t) * 10 * MATRIX_M);
  float32_t* resultIntVec = malloc(sizeof(int) * MATRIX_M);

  multiply_matrix_scalar(A, result, -2.85, MATRIX_M, MATRIX_N);
  printf("\033[33mScalar multiplication A\n");
  print_matrix(result, MATRIX_M, MATRIX_N);

  relu_matrix(A, result, MATRIX_M, MATRIX_N);
  printf("\033[34mReLU A\n");
  print_matrix(result, MATRIX_M, MATRIX_N);
  
  sum_matrix(A, B, result, MATRIX_M, MATRIX_N);
  printf("\033[35mSum A + B\n");
  print_matrix(result, MATRIX_M, MATRIX_N);

  diff_matrix(A, B, result, MATRIX_M, MATRIX_N);
  printf("\033[31mdiff A - B\n");
  print_matrix(result, MATRIX_M, MATRIX_N);

  relu_derivate_matrix(A, result, MATRIX_M, MATRIX_N);
  printf("\033[34mReLU derivate A\n");
  print_matrix(result, MATRIX_M, MATRIX_N);
  
  sum_multiply_matrix_scalar_fast(altA, B, 5.7, MATRIX_M, MATRIX_N);
  // printf("\033[33mA + x * B\n");
  // print_matrix(altA, MATRIX_M, MATRIX_N);
  
  transpose_matrix(B, result, MATRIX_M, MATRIX_N);
  printf("\033[31mTraspose B\n");
  print_matrix(result, MATRIX_N, MATRIX_M);

  minmax_matrix(A, result, MATRIX_M, MATRIX_N);
  printf("\033[33mMinMax A\n");
  print_matrix(result, MATRIX_M, MATRIX_N);

  multiply_matrix_matrix(A, D, MATRIX_M, MATRIX_N, MATRIX_O); 
  printf("\033[34mA x D\n");
  print_matrix(resultMult, MATRIX_M, MATRIX_O);

  matrix_redux_float(B, resultReduxB, MATRIX_M, MATRIX_N);
  printf("\033[31mredux B\n");
  print_matrix(resultReduxB, MATRIX_M, 1);

  one_hot_matrix(intA, 10, MATRIX_M);
  printf("\033[35moneHot A\n");
  print_matrix(oneHotA, 10, Matrix_M);

  multiply_matrix_hadamard(A, B, result, MATRIX_M, MATRIX_N);
  printf("\033[35mHadamard A B\n");
  print_matrix(result, MATRIX_M, MATRIX_N);

  int * compareAB = compare_vector(intA, intB, resultIntVec,  MATRIX_M);
  printf("\033[35mCompare intA intB\n");
  print_int_matrix(resultIntVec, MATRIX_M, 1);

  int * reduxIntMA = matrix_redux_int(intMA, resultIntVec, MATRIX_M, MATRIX_N);
  printf("\033[33mredux intMA\n");
  print_int_matrix(resultIntVec, MATRIX_M, 1);

  sum_matrix_vector(A, fvec, result, MATRIX_M, MATRIX_N);
  printf("\033[34mSum A fvec\n");
  print_matrix(sumAvec, result, MATRIX_M, MATRIX_N);

  printf("\033[0m");

  free(A);
  free(B);
  free(fvec);
  free(altA);
  free(D);
  free(intA);
  free(intB);
  free(intMA);

  return 0;
}
