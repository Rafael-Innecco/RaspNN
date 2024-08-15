#include "neural_network.h"

#include <arm_neon.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_params(neural_network_t* cnn) {
  // init_matrix(cnn->W1, 0.5, OUTPUT_LAYER_SIZE, INPUT_LAYER_SIZE);
  // init_matrix(cnn->b1, 0.5, OUTPUT_LAYER_SIZE, 1);
  init_matrix_random(cnn->W1, OUTPUT_LAYER_SIZE, INPUT_LAYER_SIZE);
  init_matrix_random(cnn->b1, OUTPUT_LAYER_SIZE, 1);
  init_matrix_random(cnn->W2, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  init_matrix_random(cnn->b2, OUTPUT_LAYER_SIZE, 1);
  return;
}

void forward_propagation(const float32_t* X, neural_network_t* cnn,
                          neural_network_layers_t* layers, int m) {
  float32_t* M;
  // input layer
  M = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE * m);
  // transpose_matrix(X, T, INPUT_LAYER_SIZE, m);
  multiply_matrix_matrix(X, cnn->W1, M, m, INPUT_LAYER_SIZE, OUTPUT_LAYER_SIZE);
  sum_matrix_vector(M, cnn->b1, layers->Z1, m, OUTPUT_LAYER_SIZE);
  softmax_matrix(layers->Z1, layers->A1, m, OUTPUT_LAYER_SIZE);
  free(M);
  return;
}

void backward_propagation(const float32_t* X, const float32_t* Y,
                           neural_network_t* cnn,
                           neural_network_gradient_t* dcnn,
                           neural_network_layers_t* layers, int m) {
  float32_t *dZ, *dZT, *XT;
  // Calculo dos dZ's
  dZ = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE * m);
  diff_matrix(layers->A1, Y, dZ, m, OUTPUT_LAYER_SIZE);
  //    Calculo das saidas
  //    dW1
  dZT = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE * m);
  transpose_matrix(dZ, dZT, m, OUTPUT_LAYER_SIZE);
  XT = malloc(sizeof(float32_t) * INPUT_LAYER_SIZE * m);
  transpose_matrix(X, XT, m, INPUT_LAYER_SIZE);
  multiply_matrix_matrix(dZT, XT, dcnn->dW1, OUTPUT_LAYER_SIZE, m,
                         INPUT_LAYER_SIZE);
  //  dB1
  matrix_redux_float(dZT, dcnn->db1, OUTPUT_LAYER_SIZE, m);
  free(dZT);
  free(XT);
  free(dZ);
  return;
}

void parameter_update(neural_network_t* cnn, neural_network_gradient_t* dcnn,
                      int m) {
  float32_t rate = ((float32_t)-LEARNING_RATE) / ((float32_t)m);
  sum_multiply_matrix_scalar_fast(cnn->W1, dcnn->dW1, rate, OUTPUT_LAYER_SIZE,
                                  INPUT_LAYER_SIZE);
  sum_multiply_matrix_scalar_fast(cnn->b1, dcnn->db1, rate, 1,
                                  OUTPUT_LAYER_SIZE);
  return;
}

void get_predictions(const float32_t* output_layer, int* predictions, int m) {
  for (int i = 0; i < m; i++) {
    float32_t max = -1.0 * FLOAT_MAX;
    for (int j = 0; j < OUTPUT_LAYER_SIZE; j++) {
      if (output_layer[OUTPUT_LAYER_SIZE * i + j] > max) {
        predictions[i] = j;
        max = output_layer[OUTPUT_LAYER_SIZE * i + j];
      }
    }
  }
  return;
}

int get_accuracy(const int* predictions, const int* expected, int m) {
  int *accuracy_vec, *compare, accuracy;
  compare = malloc(sizeof(int) * m);
  accuracy_vec = malloc(sizeof(int));
  compare_vector(predictions, expected, compare, m);
  matrix_redux_int(compare, accuracy_vec, 1, m);
  accuracy = *accuracy_vec;
  free(accuracy_vec);
  free(compare);
  return accuracy;
}

int init_batch(float32_t* X, int* Y, batch_t* batch, int index, int set_size) {
  int offset = BATCH_SIZE * index;

  batch->X = &X[offset * INPUT_LAYER_SIZE];
  batch->Y = &Y[offset];

  if (offset + BATCH_SIZE > set_size) {
    return set_size - offset;
  }

  return BATCH_SIZE;
}

int train(float32_t* X, int* Y, neural_network_t* cnn, const int set_size,
          const int iterations) {
  float32_t* Y_one_hot;
  int accuracy, m, number_of_batches, *predictions;
  neural_network_gradient_t dcnn;
  neural_network_layers_t layers;
  batch_t batch;
  predictions = malloc(sizeof(int) * BATCH_SIZE);
  layers.Z1 = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  layers.A1 = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  Y_one_hot = malloc(sizeof(float32_t) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  number_of_batches = set_size / BATCH_SIZE;
  random_params(cnn);
  for (int i = 0; i < iterations; i++) {
    m = init_batch(X, Y, &batch, i % number_of_batches, set_size);
    printf("Iteration: %d\n", i);
    printf("Tamanho do batch: %d\n", m);
    one_hot_matrix(batch.Y, Y_one_hot, m, OUTPUT_LAYER_SIZE);
    forward_propagation(batch.X, cnn, &layers, m);
    backward_propagation(batch.X, Y_one_hot, cnn, &dcnn, &layers, m);
    parameter_update(cnn, &dcnn, m);
    get_predictions(layers.A1, predictions, m);
    accuracy = get_accuracy(predictions, batch.Y, m);
    printf("Accuracy: %f\n", ((float32_t)accuracy) / ((float32_t)m));
  }
  free(layers.Z1);
  free(layers.A1);
  free(Y_one_hot);
  free(predictions);
  return accuracy;
}

int* inference(const float32_t* X, neural_network_t* cnn, const int set_size) {
  neural_network_layers_t layers;
  int* predictions;
  layers.Z1 = malloc(sizeof(float32_t) * HIDDEN_LAYER1_SIZE * set_size);
  layers.A1 = malloc(sizeof(float32_t) * HIDDEN_LAYER1_SIZE * set_size);
  forward_propagation(X, cnn, &layers, set_size);
  predictions = malloc(sizeof(int) * set_size);
  get_predictions(layers.A1, predictions, set_size);
  free(layers.Z1);
  free(layers.A1);
  return predictions;
}
