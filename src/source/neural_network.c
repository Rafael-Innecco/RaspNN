#include "neural_network.h"

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_init() {  // cria func. de incializacao aleatoria das camadas

  return;
}

void forward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_bias, float32_t* input_layer_pre_activation,
    float32_t* hidden_layer1, float32_t* hidden_layer1_weight,
    float32_t* hidden_layer1_bias, float32_t* hidden_layer1_pre_activation,
    float32_t* hidden_layer2, float32_t* hidden_layer2_weight,
    float32_t* hidden_layer2_bias, float32_t* hidden_layer2_pre_activation,
    float32_t* output_layer, int m) {
  float* Z1;
  // input layer->layer M
  Z1 = multiply_matrix_matrix(input_layer_weight, input_layer,
                              HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE,
                              m);  // Z1 = W*A
  input_layer_pre_activation =
      sum_matrix(Z1, input_layer_bias, HIDDEN_LAYER1_SIZE, m);  // Z2 = W*A + B
  hidden_layer1 = relu_matrix(input_layer_pre_activation, HIDDEN_LAYER1_SIZE,
                              m);  // Hidden1 = relu(W*A + B);
  free(Z1);
  // layer M -> layer 2
  Z1 = multiply_matrix_matrix(hidden_layer1_weight, hidden_layer1,
                              HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE,
                              m);  // Z1 = W*A
  hidden_layer1_pre_activation =
      sum_matrix(Z1, hidden_layer1_bias, HIDDEN_LAYER2_SIZE,
                 m);  // Z2 = W*A + B
  hidden_layer2 = relu_matrix(hidden_layer1_pre_activation, HIDDEN_LAYER2_SIZE,
                              m);  // Hidden2 = relu(W*A + B);
  free(Z1);
  // layer 2 -> output layer
  Z1 = multiply_matrix_matrix(hidden_layer2_weight, hidden_layer2,
                              OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE,
                              m);  // Z1 = W*A
  hidden_layer2_pre_activation =
      sum_matrix(Z1, hidden_layer2_bias, OUTPUT_LAYER_SIZE,
                 m);  // input_layer_pre_activation = W*A + B
  output_layer = minmax_matrix(hidden_layer2_pre_activation, OUTPUT_LAYER_SIZE,
                               m);  // Output = softmax(W*A + B);
  free(Z1);
  return;
}

void backward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_weight_diff, float32_t* input_layer_bias_diff,
    float32_t* input_layer_pre_activation, float32_t* hidden_layer1,
    float32_t* hidden_layer1_weight, float32_t* hidden_layer1_weight_diff,
    float32_t* hidden_layer1_bias_diff, float32_t* hidden_layer1_pre_activation,
    float32_t* hidden_layer2, float32_t* hidden_layer2_weight,
    float32_t* hidden_layer2_weight_diff, float32_t* hidden_layer2_bias_diff,
    float32_t* output_layer, float32_t* expected_layer, int m) {
  float *dZ1, *dZ2, *dZ3, *temp, *temp2, *temp3, *dWT, *dZT;
  // Calculo dos dZ's
  dZ3 = diff_matrix(output_layer, expected_layer, OUTPUT_LAYER_SIZE, m);
  dWT = transpose_matrix(hidden_layer2_weight, OUTPUT_LAYER_SIZE,
                         HIDDEN_LAYER2_SIZE);
  dZT = transpose_matrix(dZ3, OUTPUT_LAYER_SIZE, m);
  temp = multiply_matrix_matrix(dWT, dZT, OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE,
                                m);
  temp2 =
      relu_derivate_matrix(hidden_layer1_pre_activation, HIDDEN_LAYER2_SIZE, m);
  temp3 = transpose_matrix(temp2, HIDDEN_LAYER2_SIZE, m);
  dZ2 = multiply_matrix_matrix(temp, temp3, OUTPUT_LAYER_SIZE, m,
                               HIDDEN_LAYER2_SIZE);
  free(dWT);
  free(dZT);
  free(temp);
  free(temp2);
  free(temp3);
  dWT = transpose_matrix(hidden_layer1_weight, HIDDEN_LAYER2_SIZE,
                         HIDDEN_LAYER1_SIZE);
  dZT = transpose_matrix(dZ2, HIDDEN_LAYER2_SIZE, m);
  temp = multiply_matrix_matrix(dWT, dZT, HIDDEN_LAYER2_SIZE,
                                HIDDEN_LAYER1_SIZE, m);
  temp2 =
      relu_derivate_matrix(input_layer_pre_activation, HIDDEN_LAYER1_SIZE, m);
  temp3 = transpose_matrix(temp2, HIDDEN_LAYER1_SIZE, m);
  dZ1 = multiply_matrix_matrix(temp, temp3, HIDDEN_LAYER2_SIZE, m,
                               HIDDEN_LAYER1_SIZE);
  free(dWT);
  free(dZT);
  free(temp);
  free(temp2);
  free(temp3);
  // Calculo das saidas
  temp = multiply_matrix_matrix(dZ3, hidden_layer2, OUTPUT_LAYER_SIZE, m,
                                HIDDEN_LAYER2_SIZE);
  hidden_layer2_weight_diff = multiply_matrix_scalar(
      temp, 1 / m, OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE);
  free(temp);
  temp = matrix_redux(dZ3, OUTPUT_LAYER_SIZE, m);
  hidden_layer2_bias_diff =
      multiply_matrix_scalar(temp, 1 / m, OUTPUT_LAYER_SIZE, 1);
  free(temp);
  temp = multiply_matrix_matrix(dZ2, hidden_layer1, HIDDEN_LAYER2_SIZE, m,
                                HIDDEN_LAYER1_SIZE);
  hidden_layer1_weight_diff = multiply_matrix_scalar(
      temp, 1 / m, HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  free(temp);
  temp = matrix_redux(dZ2, HIDDEN_LAYER2_SIZE, m);
  hidden_layer1_bias_diff =
      multiply_matrix_scalar(temp, 1 / m, HIDDEN_LAYER2_SIZE, 1);
  free(temp);
  temp = multiply_matrix_matrix(dZ1, input_layer, INPUT_LAYER_SIZE, m,
                                HIDDEN_LAYER1_SIZE);
  input_layer_weight_diff =
      multiply_matrix_scalar(temp, 1 / m, INPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  free(temp);
  temp = matrix_redux(dZ1, INPUT_LAYER_SIZE, m);
  input_layer_bias_diff =
      multiply_matrix_scalar(temp, 1 / m, INPUT_LAYER_SIZE, 1);
  free(temp);
  free(dZ3);
  free(dZ2);
  free(dZ1);
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

float32_t* get_predictions(const float32_t* output_layer, int m) {
  float32_t* predictions = malloc(sizeof(float32_t) * m);
  float32_t* output_layer_transposted =
      transpose_matrix(output_layer, OUTPUT_LAYER_SIZE, m);
  float32_t* vec;
  for (int i = 0; i < m; i++) {
    copy_vector(vec, output_layer_transposted + OUTPUT_LAYER_SIZE * i,
                OUTPUT_LAYER_SIZE);
    max_vector_fast(vec, OUTPUT_LAYER_SIZE);
    predictions[i] = vec[0];
  }
  return predictions;
}

float32_t get_accuracy(float32_t* predictions, float32_t* expected, int m) {
  float32_t* accuracy;
  float32_t* compare = compare_vector(predictions, expected, m);
  accuracy = matrix_redux(compare, 1, m);
  return (*accuracy) / m;
}