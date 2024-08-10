
#include <stdio.h>
#include <stdlib.h>

#include "neural_network.h"
#include "uart_fsm.h"

int main2() {
  // CNN Variables
  float32_t *data_set;
  float32_t *input_layer_weight, *input_layer_bias;
  float32_t *hidden_layer1_weight, *hidden_layer1_bias;
  float32_t *hidden_layer2_weight, *hidden_layer2_bias;
  int *expected_output, *result, accuracy;
  int data_set_size, iterations;

  // UART Variables
  int state, handler, mode, action_result;

  handler = uart_init();
  state = IDLE;

  while (1) {
    action_result = action(state, handler, result, data_set_size, data_set);
    switch (state) {
      case IDLE:
        mode = action_result;
        break;
      case WAIT_TRAIN_DATA:
        data_set_size = action_result;
        break;
      case WAIT_TRAIN_RESULT:
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
      case SEND_INFERENCE_RESULT:
        free(result);
    }
    state = next_state(state, mode);
  }

  return 0;
}