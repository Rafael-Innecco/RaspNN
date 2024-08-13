#include "neural_network.h"

#include <arm_neon.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_params(float32_t** input_layer_weight, float32_t** input_layer_bias,
                   float32_t** hidden_layer1_weight,
                   float32_t** hidden_layer1_bias,
                   float32_t** hidden_layer2_weight,
                   float32_t** hidden_layer2_bias) {
  *input_layer_weight =
      init_matrix_random(HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE);
  *input_layer_bias = init_matrix_random(HIDDEN_LAYER1_SIZE, 1);
  *hidden_layer1_weight =
      init_matrix_random(HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  *hidden_layer1_bias = init_matrix_random(HIDDEN_LAYER2_SIZE, 1);
  *hidden_layer2_weight =
      init_matrix_random(OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE);
  *hidden_layer2_bias = init_matrix_random(OUTPUT_LAYER_SIZE, 1);
  return;
}

void forward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_bias, float32_t** input_layer_pre_activation,
    float32_t** hidden_layer1, float32_t* hidden_layer1_weight,
    float32_t* hidden_layer1_bias, float32_t** hidden_layer1_pre_activation,
    float32_t** hidden_layer2, float32_t* hidden_layer2_weight,
    float32_t* hidden_layer2_bias, float32_t** hidden_layer2_pre_activation,
    float32_t** output_layer, int m) {
  float32_t *Z, *transposed;
  // input layer
  transposed = transpose_matrix(input_layer, INPUT_LAYER_SIZE, m);
  Z = multiply_matrix_matrix(input_layer_weight, transposed, HIDDEN_LAYER1_SIZE,
                             INPUT_LAYER_SIZE, m);
  *input_layer_pre_activation =
      sum_matrix_vector(Z, input_layer_bias, HIDDEN_LAYER1_SIZE, m);
  *hidden_layer1 =
      relu_matrix(*input_layer_pre_activation, HIDDEN_LAYER1_SIZE, m);
  free(transposed);
  free(Z);
  // hidden layer 1
  transposed = transpose_matrix(*hidden_layer1, HIDDEN_LAYER1_SIZE, m);
  Z = multiply_matrix_matrix(hidden_layer1_weight, transposed,
                             HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE, m);
  *hidden_layer1_pre_activation =
      sum_matrix_vector(Z, hidden_layer1_bias, HIDDEN_LAYER2_SIZE, m);
  *hidden_layer2 =
      relu_matrix(*hidden_layer1_pre_activation, HIDDEN_LAYER2_SIZE, m);
  free(transposed);
  free(Z);
  // hidden layer 2
  transposed = transpose_matrix(*hidden_layer2, HIDDEN_LAYER2_SIZE, m);
  Z = multiply_matrix_matrix(hidden_layer2_weight, transposed,
                             OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE, m);
  *hidden_layer2_pre_activation =
      sum_matrix_vector(Z, hidden_layer2_bias, OUTPUT_LAYER_SIZE, m);
  *output_layer =
      minmax_matrix(*hidden_layer2_pre_activation, OUTPUT_LAYER_SIZE, m);
  free(transposed);
  free(Z);
  return;
}

void backward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t** input_layer_weight_diff, float32_t** input_layer_bias_diff,
    float32_t* input_layer_pre_activation, float32_t* hidden_layer1,
    float32_t* hidden_layer1_weight, float32_t** hidden_layer1_weight_diff,
    float32_t** hidden_layer1_bias_diff,
    float32_t* hidden_layer1_pre_activation, float32_t* hidden_layer2,
    float32_t* hidden_layer2_weight, float32_t** hidden_layer2_weight_diff,
    float32_t** hidden_layer2_bias_diff, float32_t* output_layer,
    float32_t* expected_layer, int m) {
  float32_t *dZ1, *dZ2, *dZ3, *temp, *relu, *dWT, *dZT;
  // Calculo dos dZ's
  dZ3 = diff_matrix(output_layer, expected_layer, OUTPUT_LAYER_SIZE, m);
  dWT = transpose_matrix(hidden_layer2_weight, OUTPUT_LAYER_SIZE,
                         HIDDEN_LAYER2_SIZE);
  dZT = transpose_matrix(dZ3, OUTPUT_LAYER_SIZE, m);
  temp = multiply_matrix_matrix(dWT, dZT, HIDDEN_LAYER2_SIZE, OUTPUT_LAYER_SIZE,
                                m);
  relu =
      relu_derivate_matrix(hidden_layer1_pre_activation, HIDDEN_LAYER2_SIZE, m);
  dZ2 = multiply_matrix_hadamard(temp, relu, HIDDEN_LAYER2_SIZE, m);
  free(dWT);
  free(dZT);
  free(temp);
  free(relu);
  dWT = transpose_matrix(hidden_layer1_weight, HIDDEN_LAYER2_SIZE,
                         HIDDEN_LAYER1_SIZE);
  dZT = transpose_matrix(dZ2, HIDDEN_LAYER2_SIZE, m);
  temp = multiply_matrix_matrix(dWT, dZT, HIDDEN_LAYER1_SIZE,
                                HIDDEN_LAYER2_SIZE, m);
  relu =
      relu_derivate_matrix(input_layer_pre_activation, HIDDEN_LAYER1_SIZE, m);
  dZ1 = multiply_matrix_hadamard(temp, relu, HIDDEN_LAYER1_SIZE, m);
  free(dWT);
  free(dZT);
  free(temp);
  free(relu);
  // Calculo das saidas
  temp = multiply_matrix_matrix(dZ3, hidden_layer2, OUTPUT_LAYER_SIZE, m,
                                HIDDEN_LAYER2_SIZE);
  float32_t m_inv = ((float32_t)1) / ((float32_t)m);
  *hidden_layer2_weight_diff = multiply_matrix_scalar(
      temp, m_inv, OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE);
  free(temp);
  temp = matrix_redux_float(dZ3, OUTPUT_LAYER_SIZE, m);
  *hidden_layer2_bias_diff =
      multiply_matrix_scalar(temp, m_inv, OUTPUT_LAYER_SIZE, 1);
  free(temp);
  temp = multiply_matrix_matrix(dZ2, hidden_layer1, HIDDEN_LAYER2_SIZE, m,
                                HIDDEN_LAYER1_SIZE);
  *hidden_layer1_weight_diff = multiply_matrix_scalar(
      temp, m_inv, HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  free(temp);
  temp = matrix_redux_float(dZ2, HIDDEN_LAYER2_SIZE, m);
  *hidden_layer1_bias_diff =
      multiply_matrix_scalar(temp, m_inv, HIDDEN_LAYER2_SIZE, 1);
  free(temp);
  temp = multiply_matrix_matrix(dZ1, input_layer, HIDDEN_LAYER1_SIZE, m,
                                INPUT_LAYER_SIZE);
  *input_layer_weight_diff =
      multiply_matrix_scalar(temp, m_inv, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE);
  free(temp);
  temp = matrix_redux_float(dZ1, HIDDEN_LAYER1_SIZE, m);
  *input_layer_bias_diff =
      multiply_matrix_scalar(temp, m_inv, HIDDEN_LAYER1_SIZE, 1);
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
                                  -LEARNING_RATE, HIDDEN_LAYER1_SIZE, 1);
  sum_multiply_matrix_scalar_fast(hidden_layer1_weight,
                                  hidden_layer1_weight_diff, -LEARNING_RATE,
                                  HIDDEN_LAYER2_SIZE, HIDDEN_LAYER1_SIZE);
  sum_multiply_matrix_scalar_fast(hidden_layer1_bias, hidden_layer1_bias_diff,
                                  -LEARNING_RATE, HIDDEN_LAYER2_SIZE, 1);
  sum_multiply_matrix_scalar_fast(hidden_layer2_weight,
                                  hidden_layer2_weight_diff, -LEARNING_RATE,
                                  OUTPUT_LAYER_SIZE, HIDDEN_LAYER2_SIZE);
  sum_multiply_matrix_scalar_fast(hidden_layer2_bias, hidden_layer2_bias_diff,
                                  -LEARNING_RATE, OUTPUT_LAYER_SIZE, 1);
  return;
}

void get_predictions(const float32_t* output_layer, int* predictions, int m) {
  float32_t* output_layer_transposed =
      transpose_matrix(output_layer, OUTPUT_LAYER_SIZE, m);
  float32_t* vec = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE);
  for (int i = 0; i < m; i++) {
    copy_vector(output_layer_transposed + OUTPUT_LAYER_SIZE * i, vec,
                OUTPUT_LAYER_SIZE);
    predictions[i] = (int)max_vector_fast(vec, OUTPUT_LAYER_SIZE);
  }
  free(vec);
  free(output_layer_transposed);
  return;
}

int get_accuracy(const int* predictions, const int* expected, int m) {
  int *accuracy_vec, accuracy;
  int* compare = compare_vector(predictions, expected, m);
  accuracy_vec = matrix_redux_int(compare, 1, m);
  accuracy = *accuracy_vec;
  free(accuracy_vec);
  return accuracy;
}

int train(float32_t* train_set, int* expected_output,
          float32_t** input_layer_weight, float32_t** input_layer_bias,
          float32_t** hidden_layer1_weight, float32_t** hidden_layer1_bias,
          float32_t** hidden_layer2_weight, float32_t** hidden_layer2_bias,
          const int train_set_size, const int iterations) {
  random_params(input_layer_weight, input_layer_bias, hidden_layer1_weight,
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
  int* predictions = malloc(sizeof(int) * train_set_size);
  int accuracy = 0;
  for (int i = 0; i < iterations; i++) {
    forward_propagation(
        train_set, *input_layer_weight, *input_layer_bias,
        &input_layer_pre_activation, &hidden_layer1, *hidden_layer1_weight,
        *hidden_layer1_bias, &hidden_layer1_pre_activation, &hidden_layer2,
        *hidden_layer2_weight, *hidden_layer2_bias,
        &hidden_layer2_pre_activation, &output_layer, train_set_size);
    backward_propagation(train_set, *input_layer_weight,
                         &input_layer_weight_diff, &input_layer_bias_diff,
                         input_layer_pre_activation, hidden_layer1,
                         *hidden_layer1_weight, &hidden_layer1_weight_diff,
                         &hidden_layer1_bias_diff, hidden_layer1_pre_activation,
                         hidden_layer2, *hidden_layer2_weight,
                         &hidden_layer2_weight_diff, &hidden_layer2_bias_diff,
                         output_layer, expected_output_one_hot, train_set_size);
    parameter_update(*input_layer_weight, input_layer_weight_diff,
                     *input_layer_bias, input_layer_bias_diff,
                     *hidden_layer1_weight, hidden_layer1_weight_diff,
                     *hidden_layer1_bias, hidden_layer1_bias_diff,
                     *hidden_layer2_weight, hidden_layer2_weight_diff,
                     *hidden_layer2_bias, hidden_layer2_bias_diff);
    if (i % 10 == 0) {
      printf("Iteration: %d\n", i);
      get_predictions(output_layer, predictions, train_set_size);
      accuracy = get_accuracy(predictions, expected_output, train_set_size);
      printf("Accuracy: %f\n",
             ((float32_t)accuracy) / ((float32_t)train_set_size));
    }
    free(hidden_layer1);
    free(hidden_layer2);
    free(output_layer);
    free(input_layer_pre_activation);
    free(hidden_layer1_pre_activation);
    free(hidden_layer2_pre_activation);
    free(input_layer_weight_diff);
    free(hidden_layer1_weight_diff);
    free(hidden_layer2_weight_diff);
    free(input_layer_bias_diff);
    free(hidden_layer1_bias_diff);
    free(hidden_layer2_bias_diff);
  }
  free(expected_output_one_hot);
  free(predictions);
  return accuracy;
}

int* inference(float32_t* input_layer, float32_t* input_layer_weight,
               float32_t* input_layer_bias, float32_t* hidden_layer1_weight,
               float32_t* hidden_layer1_bias, float32_t* hidden_layer2_weight,
               float32_t* hidden_layer2_bias, const int inference_set_size) {
  float32_t *hidden_layer1, *hidden_layer2, *output_layer;
  float32_t *input_layer_pre_activation, *hidden_layer1_pre_activation,
      *hidden_layer2_pre_activation;
  int* predictions;
  forward_propagation(
      input_layer, input_layer_weight, input_layer_bias,
      &input_layer_pre_activation, &hidden_layer1, hidden_layer1_weight,
      hidden_layer1_bias, &hidden_layer1_pre_activation, &hidden_layer2,
      hidden_layer2_weight, hidden_layer2_bias, &hidden_layer2_pre_activation,
      &output_layer, inference_set_size);
  predictions = malloc(sizeof(int) * inference_set_size);
  get_predictions(output_layer, predictions, inference_set_size);
  free(hidden_layer1);
  free(hidden_layer2);
  free(output_layer);
  free(input_layer_pre_activation);
  free(hidden_layer1_pre_activation);
  free(hidden_layer2_pre_activation);
  return predictions;
}
