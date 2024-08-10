#include "neural_network.h"

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_params(float32_t* input_layer_weight, float32_t* input_layer_bias,
                   float32_t* hidden_layer1_weight,
                   float32_t* hidden_layer1_bias,
                   float32_t* hidden_layer2_weight,
                   float32_t* hidden_layer2_bias) {
  input_layer_weight = init_matrix_random(HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE);
  input_layer_bias = init_matrix_random(HIDDEN_LAYER1_SIZE, 1);
  hidden_layer1_weight =
      init_matrix_random(HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  hidden_layer1_bias = init_matrix_random(HIDDEN_LAYER2_SIZE, 1);
  hidden_layer2_weight =
      init_matrix_random(OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE);
  hidden_layer2_bias = init_matrix_random(OUTPUT_LAYER_SIZE, 1);
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
  temp = matrix_redux_float(dZ3, OUTPUT_LAYER_SIZE, m);
  hidden_layer2_bias_diff =
      multiply_matrix_scalar(temp, 1 / m, OUTPUT_LAYER_SIZE, 1);
  free(temp);
  temp = multiply_matrix_matrix(dZ2, hidden_layer1, HIDDEN_LAYER2_SIZE, m,
                                HIDDEN_LAYER1_SIZE);
  hidden_layer1_weight_diff = multiply_matrix_scalar(
      temp, 1 / m, HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  free(temp);
  temp = matrix_redux_float(dZ2, HIDDEN_LAYER2_SIZE, m);
  hidden_layer1_bias_diff =
      multiply_matrix_scalar(temp, 1 / m, HIDDEN_LAYER2_SIZE, 1);
  free(temp);
  temp = multiply_matrix_matrix(dZ1, input_layer, INPUT_LAYER_SIZE, m,
                                HIDDEN_LAYER1_SIZE);
  input_layer_weight_diff =
      multiply_matrix_scalar(temp, 1 / m, INPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  free(temp);
  temp = matrix_redux_float(dZ1, INPUT_LAYER_SIZE, m);
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

void train_network(float32_t* train_set, const int* expected_output,
                   float32_t* input_layer_weight, float32_t* input_layer_bias,
                   float32_t* hidden_layer1_weight,
                   float32_t* hidden_layer1_bias,
                   float32_t* hidden_layer2_weight,
                   float32_t* hidden_layer2_bias, const int train_set_size,
                   const int iterations) {
  random_params(input_layer_weight, input_layer_weight, hidden_layer1_weight,
                hidden_layer1_bias, hidden_layer2_weight, hidden_layer2_bias);
  float32_t* expected_output_one_hot =
      one_hot_matrix(expected_output, OUTPUT_LAYER_SIZE, train_set_size);
  float32_t *hidden_layer1, *hidden_layer2, *output_layer;
  float32_t *input_layer_pre_activation, *hidden_layer1_pre_activation,
      *hidden_layer2_pre_activation;
  float32_t *input_layer_weight_diff, *hidden_layer1_weight_diff,
      *hidden_layer2_weight_diff;
  float32_t *input_layer_bias_diff, *hidden_layer1_bias_diff,
      *hidden_layer2_bias_diff;
  for (int i = 0; i < iterations; i++) {
    forward_propagation(
        train_set, input_layer_weight, input_layer_bias,
        input_layer_pre_activation, hidden_layer1, hidden_layer1_weight,
        hidden_layer1_bias, hidden_layer1_pre_activation, hidden_layer2,
        hidden_layer2_weight, hidden_layer2_bias, hidden_layer2_pre_activation,
        output_layer, train_set_size);
    backward_propagation(train_set, input_layer_weight, input_layer_weight_diff,
                         input_layer_bias_diff, input_layer_pre_activation,
                         hidden_layer1, hidden_layer1_weight,
                         hidden_layer1_weight_diff, hidden_layer1_bias_diff,
                         hidden_layer1_pre_activation, hidden_layer2,
                         hidden_layer2_weight, hidden_layer2_weight_diff,
                         hidden_layer2_bias_diff, output_layer,
                         expected_output_one_hot, train_set_size);
    parameter_update(
        input_layer_weight, input_layer_weight_diff, input_layer_bias,
        input_layer_bias_diff, hidden_layer1_weight, hidden_layer1_weight_diff,
        hidden_layer1_bias, hidden_layer1_bias_diff, hidden_layer2_weight,
        hidden_layer2_weight_diff, hidden_layer2_bias, hidden_layer2_bias_diff);
    if (i % 10 == 0) {
      printf("Iteration: %d\n", i);
      int* predictions = get_predictions(output_layer, train_set_size);
      printf("Accuracy: %f",
             get_accuracy(predictions, expected_output, train_set_size));
    }
  }
  return;
}

int* get_predictions(const float32_t* output_layer, int m) {
  int* predictions = malloc(sizeof(float32_t) * m);
  float32_t* output_layer_transposted =
      transpose_matrix(output_layer, OUTPUT_LAYER_SIZE, m);
  float32_t* vec;
  for (int i = 0; i < m; i++) {
    copy_vector(vec, output_layer_transposted + OUTPUT_LAYER_SIZE * i,
                OUTPUT_LAYER_SIZE);
    max_vector_fast(vec, OUTPUT_LAYER_SIZE);
    predictions[i] = (int)vec[0];
  }
  return predictions;
}

float32_t get_accuracy(const int* predictions, const int* expected, int m) {
  int* accuracy;
  int* compare = compare_vector(predictions, expected, m);
  accuracy = matrix_redux_int(compare, 1, m);
  return (*accuracy) / m;
}