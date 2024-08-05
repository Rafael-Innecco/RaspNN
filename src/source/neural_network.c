#include "neural_network.h"

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_init() {  // cria func. de incializacao aleatoria das camadas

  return;
}

void forward_propagation(float32_t* input_layer, float32_t* input_layer_weight,
                         float32_t* input_layer_bias, float32_t* hidden_layer1,
                         float32_t* hidden_layer1_weight,
                         float32_t* hidden_layer1_bias,
                         float32_t* hidden_layer2,
                         float32_t* hidden_layer2_weight,
                         float32_t* hidden_layer2_bias, float32_t* output_layer,
                         int m) {
  float *Z1, *Z2;
  // input layer->layer M
  Z1 = multiply_matrix_matrix(input_layer_weight, input_layer,
                              HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE,
                              m);                                // Z1 = W*A
  Z2 = sum_matrix(Z1, input_layer_bias, HIDDEN_LAYER1_SIZE, m);  // Z2 = W*A + B
  hidden_layer1 =
      relu_matrix(Z2, HIDDEN_LAYER1_SIZE, m);  // Hidden1 = relu(W*A + B);
  free(Z1);
  free(Z2);
  // layer M -> layer 2
  Z1 = multiply_matrix_matrix(hidden_layer1_weight, hidden_layer1,
                              HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE,
                              m);  // Z1 = W*A
  Z2 = sum_matrix(Z1, hidden_layer1_bias, HIDDEN_LAYER2_SIZE,
                  m);  // Z2 = W*A + B
  hidden_layer2 =
      relu_matrix(Z2, HIDDEN_LAYER2_SIZE, m);  // Hidden2 = relu(W*A + B);
  free(Z1);
  free(Z2);
  // layer 2 -> output layer
  Z1 = multiply_matrix_matrix(hidden_layer2_weight, hidden_layer2,
                              OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE,
                              m);  // Z1 = W*A
  Z2 =
      sum_matrix(Z1, hidden_layer2_bias, OUTPUT_LAYER_SIZE, m);  // Z2 = W*A + B
  output_layer =
      minmax_matrix(Z2, OUTPUT_LAYER_SIZE, m);  // Output = softmax(W*A + B);
  free(Z1);
  free(Z2);
  return;
}

void backward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_weight_diff, float32_t* input_layer_bias_diff,
    float32_t* hidden_layer1, float32_t* hidden_layer1_weight,
    float32_t* hidden_layer1_weight_diff, float32_t* hidden_layer1_bias_diff,
    float32_t* hidden_layer2, float32_t* hidden_layer2_weight,
    float32_t* hidden_layer2_weight_diff, float32_t* hidden_layer2_bias_diff,
    float32_t* output_layer, float32_t* expected_layer, int m) {
  float *dZ1, *dZ2, *dZ3;
  dZ3 = diff_matrix(output_layer, expected_layer, OUTPUT_LAYER_SIZE, m);

  return;
}

void parameter_update(
    float32_t* input_layer_weight, float32_t* input_layer_weight_diff,
    float32_t* input_layer_bias, float32_t* input_layer_bias_diff,
    float32_t* hidden_layer1_weight, float32_t* hidden_layer1_weight_diff,
    float32_t* hidden_layer1_bias, float32_t* hidden_layer1_bias_diff,
    float32_t* hidden_layer2_weight, float32_t* hidden_layer2_weight_diff,
    float32_t* hidden_layer2_bias, float32_t* hidden_layer2_bias_diff) {
  sum_multiply_matrix_scalar_fast(input_layer_weight, input_layer_weight_diff,
                                  -LEARNING_RATE, HIDDEN_LAYER1_SIZE,
                                  INPUT_LAYER_SIZE);
  sum_multiply_matrix_scalar_fast(input_layer_bias, input_layer_bias_diff,
                                  -LEARNING_RATE, HIDDEN_LAYER1_SIZE,
                                  INPUT_LAYER_SIZE);
  sum_multiply_matrix_scalar_fast(hidden_layer1_weight,
                                  hidden_layer1_weight_diff, -LEARNING_RATE,
                                  HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  sum_multiply_matrix_scalar_fast(hidden_layer1_bias, hidden_layer1_bias_diff,
                                  -LEARNING_RATE, HIDDEN_LAYER2_SIZE,
                                  HIDDEN_LAYER1_SIZE);
  sum_multiply_matrix_scalar_fast(hidden_layer2_weight,
                                  hidden_layer2_weight_diff, -LEARNING_RATE,
                                  OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE);
  sum_multiply_matrix_scalar_fast(hidden_layer2_bias, hidden_layer2_bias_diff,
                                  -LEARNING_RATE, OUTPUT_LAYER_SIZE,
                                  HIDDEN_LAYER2_SIZE);
  return;
}
