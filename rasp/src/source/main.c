#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#include "neural_network.h"
#include "server_fsm.h"
#include "socket_wrapper.h"


#define MATRIX_M 63
#define MATRIX_N 63
#define MATRIX_O 9

int main_server();
int main_test();

int main() {
  printf("Started\n");
  return main_server();
}

int main_server() { // CNN Variables
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
        printf("\033[36mDataset size = %d\n", data_set_size);
        break;
      case WAIT_TRAIN_LABELS:
        expected_output = result;
        break;
      case WAIT_TRAIN_ITERATIONS:
        iterations = action_result;
        accuracy = train(data_set, expected_output, input_layer_weight,
                         input_layer_bias, hidden_layer1_weight,
                         hidden_layer1_bias, hidden_layer2_weight,
                         hidden_layer2_bias, data_set_size, iterations);
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

  float32_t * A = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t * altA = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t * B = malloc(sizeof(float32_t) * MATRIX_M * MATRIX_N);
  float32_t * D = malloc(sizeof(float32_t) * MATRIX_O * MATRIX_N);
  float32_t * fvec = malloc(sizeof(float32_t)  * MATRIX_M);

  int * intA = malloc (sizeof(int) * MATRIX_M);
  int * intB = malloc (sizeof(int) * MATRIX_M);
  int * intMA = malloc (sizeof(int) * MATRIX_M * MATRIX_N);

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

      intMA[MATRIX_N*i + j] = 13 * (i + 1) * (j + 1) % 17;
    }
    fvec[i] = (MATRIX_M * 1.0 * i) / (MATRIX_N);

    int x = MATRIX_M - i;
    intA[i] = (x*x) % 10;
    intB[i] = x * x * x % 10;
  }

  for (i = 0; i < MATRIX_O; i++) {
    for (j = 0; j < MATRIX_N; j++) {
      D[MATRIX_N*i + j] = 17 % (i + j);
    }
  }


  printf("\033[36mMatrizes operando:\n");
  print_matrix(A, MATRIX_M, MATRIX_N);
  // print_matrix(B, MATRIX_M, MATRIX_N);
  // print_matrix(fvec, MATRIX_M, 1);
  // print_matrix(D, MATRIX_O, MATRIX_N);
  // print_int_matrix(intA, MATRIX_M, 1);
  // print_int_matrix(intB, MATRIX_M, 1);
  // print_int_matrix(intMA, MATRIX_M, MATRIX_N);
  printf("\033[0m\n");

  // float32_t * scalarA = multiply_matrix_scalar(A, -2.85, MATRIX_M, MATRIX_N);
  // float32_t * reluA = relu_matrix(A, MATRIX_M, MATRIX_N);
  // float32_t * C = sum_matrix(A, B, MATRIX_M, MATRIX_N);
  // float32_t * mult = multiply_matrix_matrix(A, D, MATRIX_M, MATRIX_N, MATRIX_O);
  // float32_t * diffAB = diff_matrix(A, B, MATRIX_M, MATRIX_N);
  // float32_t * reluDevA = relu_derivate_matrix(A, MATRIX_M, MATRIX_N);
  // float32_t * oneHotA = one_hot_matrix(intA, 10, MATRIX_M);
  // float32_t * transposeB = transpose_matrix(B, MATRIX_M, MATRIX_N);
  // int * compareAB = compare_vector(intA, intB, MATRIX_M);

  // sum_multiply_matrix_scalar_fast(altA, B, 5.7, MATRIX_M, MATRIX_N);

  float32_t * minMaxA = minmax_matrix(A, MATRIX_M, MATRIX_N);
  // float32_t * reduxB = matrix_redux_float(B, MATRIX_M, MATRIX_N);
  // int * reduxIntMA = matrix_redux_int(intMA, MATRIX_M, MATRIX_N);
  float32_t * sumAvec = sum_matrix_vector(A, fvec, MATRIX_M, MATRIX_N); 
  float32_t * hadamard = multiply_matrix_hadamard(A, B, MATRIX_M, MATRIX_N);

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
  // printf("\033[31mTraspose B\n");
  // print_matrix(transposeB, MATRIX_N, MATRIX_M);
  printf("\033[33mMinMax A\n");
  print_matrix(minMaxA, MATRIX_M, MATRIX_N);
  // printf("\033[34mA x D\n");
  // print_matrix(mult, MATRIX_M, MATRIX_O);
  // printf("\033[35mCompare intA intB\n");
  // print_int_matrix(compareAB, 1, MATRIX_M);
  // printf("\033[31mredux B\n");
  // print_matrix(reduxB, MATRIX_M, 1);
  // printf("\033[33mredux intMA\n");
  // print_int_matrix(reduxIntMA, MATRIX_M, 1);
  // printf("\033[34mSum A fvec\n");
  // print_matrix(sumAvec, MATRIX_M, MATRIX_N);
  // printf("\033[35mHadamard A B\n");
  // print_matrix(hadamard, MATRIX_M, MATRIX_N);
  printf("\033[0m");


  free(A);
  free(B);
  free(fvec);
  free(altA);
  free(D);
  free(intA);
  free(intB);
  free(intMA);

  free(minMaxA);
  free(sumAvec);
  free(hadamard);
  // free(transposeB);
  return 0;
}
