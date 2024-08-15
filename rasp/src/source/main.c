#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "neural_network.h"
#include "server_fsm.h"
#include "socket_wrapper.h"

#define MATRIX_M 13
#define MATRIX_N 11
#define MATRIX_O 9

int main() {
  float *data;
  neural_network_t cnn;
  int *expected_output, *result;
  int data_set_size, iterations;
  float32_t accuracy;

  // FSM Variables
  int state, sock, mode, action_result, server_fd;

  state = INIT;

  while (1) {
    action_result = action(state, sock, result, data_set_size, &data);
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
        iterations = action_result;
        accuracy =
            train(data, expected_output, &cnn, data_set_size, iterations);
        free(expected_output);
        free(data);
        data = &accuracy;
        break;
      case SEND_TRAINING_RESULT:
        break;
      case WAIT_INFERENCE_DATA:
        data_set_size = action_result;
        result = inference(data, &cnn, data_set_size);
        free(data);
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
