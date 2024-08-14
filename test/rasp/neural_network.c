#include "neural_network.h"

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_params(neural_network_t* cnn) {
  init_matrix_random(cnn->W1, OUTPUT_LAYER_SIZE, INPUT_LAYER_SIZE);
  init_matrix_random(cnn->b1, OUTPUT_LAYER_SIZE, 1);
  // init_matrix_random(cnn->W2, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  // init_matrix_random(cnn->b2, OUTPUT_LAYER_SIZE, 1);
  return;
}

void forward_propagation(const float* X, neural_network_t* cnn,
                         neural_network_layers_t* layers, int m) {
  float *T, *M;
  // input layer
  T = malloc(sizeof(float) * INPUT_LAYER_SIZE * m);
  M = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  // print_matrix_parcial(X, 10, m, 10);
  transpose_matrix(X, T, INPUT_LAYER_SIZE, m);
  multiply_matrix_matrix(cnn->W1, T, M, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE,
                         m);
  sum_matrix_vector(M, cnn->b1, layers->Z1, HIDDEN_LAYER1_SIZE, m);
  // print_matrix_parcial(cnn->W1, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE, 10);
  // print_matrix(cnn->b1, 1, HIDDEN_LAYER1_SIZE);
  // print_matrix_parcial(layers->Z1, HIDDEN_LAYER1_SIZE, m, 10);
  relu_matrix(layers->Z1, layers->A1, HIDDEN_LAYER1_SIZE, m);
  free(T);
  free(M);
  // hidden layer 1
  T = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  M = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  transpose_matrix(layers->A1, T, HIDDEN_LAYER1_SIZE, m);
  multiply_matrix_matrix(cnn->W2, T, M, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE,
                         m);
  sum_matrix_vector(M, cnn->b2, layers->Z2, OUTPUT_LAYER_SIZE, m);
  softmax_matrix(layers->Z2, layers->A2, OUTPUT_LAYER_SIZE, m);
  free(T);
  free(M);
  return;
}

void forward_propagation2(const float* X, neural_network_t* cnn,
                          neural_network_layers_t* layers, int m) {
  float *T, *M;
  // input layer
  T = malloc(sizeof(float) * INPUT_LAYER_SIZE * m);
  M = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  transpose_matrix(X, T, INPUT_LAYER_SIZE, m);
  multiply_matrix_matrix(cnn->W1, T, M, OUTPUT_LAYER_SIZE, INPUT_LAYER_SIZE, m);
  sum_matrix_vector(M, cnn->b1, layers->Z1, OUTPUT_LAYER_SIZE, m);
  softmax_matrix(layers->Z1, layers->A1, OUTPUT_LAYER_SIZE, m);
  free(T);
  free(M);
  return;
}

void backward_propagation(const float* X, const float* Y, neural_network_t* cnn,
                          neural_network_gradient_t* dcnn,
                          neural_network_layers_t* layers, int m) {
  float *dZ1, *dZ2, *M, *relu, *WT, *dZT;
  // Calculo dos dZ's
  dZ2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  diff_matrix(layers->A2, Y, dZ2, OUTPUT_LAYER_SIZE, m);
  WT = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE);
  transpose_matrix(cnn->W2, WT, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  dZT = malloc((sizeof(float) * OUTPUT_LAYER_SIZE * m));
  transpose_matrix(dZ2, dZT, OUTPUT_LAYER_SIZE, m);
  M = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  multiply_matrix_matrix(WT, dZT, M, HIDDEN_LAYER1_SIZE, OUTPUT_LAYER_SIZE, m);
  relu = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  relu_derivate_matrix(layers->Z1, relu, HIDDEN_LAYER1_SIZE, m);
  dZ1 = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  multiply_matrix_hadamard(M, relu, dZ1, HIDDEN_LAYER1_SIZE, m);
  // print_matrix_parcial(cnn->W2, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE, 10);
  fflush(stdout);
  free(WT);
  free(dZT);
  free(M);
  free(relu);
  // Calculo das saidas
  // dW2
  multiply_matrix_matrix(dZ2, layers->A1, dcnn->dW2, OUTPUT_LAYER_SIZE, m,
                         HIDDEN_LAYER1_SIZE);
  // dB2
  matrix_redux_float(dZ2, dcnn->db2, OUTPUT_LAYER_SIZE, m);
  // dW1
  // print_matrix_parcial(dZ1, HIDDEN_LAYER1_SIZE, m, 10);
  multiply_matrix_matrix(dZ1, X, dcnn->dW1, HIDDEN_LAYER1_SIZE, m,
                         INPUT_LAYER_SIZE);
  // print_matrix_parcial(dcnn->dW1, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE, 10);
  // fflush(stdout);
  // dB1
  matrix_redux_float(dZ1, dcnn->db1, HIDDEN_LAYER1_SIZE, m);
  free(dZ2);
  free(dZ1);
  return;
}

void backward_propagation2(const float* X, const float* Y,
                           neural_network_t* cnn,
                           neural_network_gradient_t* dcnn,
                           neural_network_layers_t* layers, int m) {
  float* dZ;
  // Calculo dos dZ's
  dZ = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  diff_matrix(layers->A1, Y, dZ, OUTPUT_LAYER_SIZE, m);
  // print_matrix_parcial(dZ, OUTPUT_LAYER_SIZE, m, 10);
  //  Calculo das saidas
  //  dW1
  multiply_matrix_matrix(dZ, X, dcnn->dW1, OUTPUT_LAYER_SIZE, m,
                         INPUT_LAYER_SIZE);
  // dB1
  matrix_redux_float(dZ, dcnn->db1, OUTPUT_LAYER_SIZE, m);
  free(dZ);
  return;
}

void parameter_update(neural_network_t* cnn, neural_network_gradient_t* dcnn,
                      int m) {
  float rate = ((float)-LEARNING_RATE) / ((float)m);
  // printf("Taxa de aprendizado: %.3f\n", rate);
  print_matrix(dcnn->dW1, OUTPUT_LAYER_SIZE, INPUT_LAYER_SIZE);
  sum_multiply_matrix_scalar_fast(cnn->W1, dcnn->dW1, rate, OUTPUT_LAYER_SIZE,
                                  INPUT_LAYER_SIZE);
  // print_matrix_parcial(cnn->W1, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE, 10);
  sum_multiply_matrix_scalar_fast(cnn->b1, dcnn->db1, rate, OUTPUT_LAYER_SIZE,
                                  1);
  // sum_multiply_matrix_scalar_fast(cnn->W2, dcnn->dW2, rate,
  // HIDDEN_LAYER2_SIZE,
  //                                 HIDDEN_LAYER1_SIZE);
  // sum_multiply_matrix_scalar_fast(cnn->b2, dcnn->db2, rate,
  // HIDDEN_LAYER2_SIZE,
  //                                 1);
  // print_matrix_parcial(cnn->W1, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE, 10);
  return;
}

void get_predictions(const float* output_layer, int* predictions, int m) {
  for (int i = 0; i < m; i++) {
    float max = -1.0 * FLOAT_MAX;
    for (int j = 0; j < OUTPUT_LAYER_SIZE; j++) {
      if (output_layer[m * j + i] > max) {
        predictions[i] = j;
        max = output_layer[m * j + i];
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

int init_batch(float* X, int* Y, batch_t* batch, int index, int set_size) {
  int offset = BATCH_SIZE * index;

  batch->X = &X[offset * INPUT_LAYER_SIZE];
  batch->Y = &Y[offset];

  if (offset + BATCH_SIZE > set_size) {
    return set_size - offset;
  }

  return BATCH_SIZE;
}

int train(float* X, int* Y, neural_network_t* cnn, const int set_size,
          const int iterations) {
  float* Y_one_hot;
  int accuracy, m, number_of_batches, *predictions;
  neural_network_gradient_t dcnn;
  neural_network_layers_t layers;
  batch_t batch;
  predictions = malloc(sizeof(int) * set_size);
  layers.Z1 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  layers.A1 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  // layers.Z2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  // layers.A2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  Y_one_hot = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  number_of_batches = set_size / BATCH_SIZE;
  random_params(cnn);
  for (int i = 0; i < iterations; i++) {
    m = init_batch(X, Y, &batch, i % BATCH_SIZE, set_size);
    printf("Iteration: %d\n", i);
    printf("Tamanho do batch: %d\n", m);
    // printf("Endereco dos pesos 1: %p\n", cnn->W1);
    //  print_matrix_parcial(cnn->W1, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE, 10);
    // fflush(stdout);
    one_hot_matrix(batch.Y, Y_one_hot, OUTPUT_LAYER_SIZE, m);
    // print_matrix_parcial(batch.X, 10, m, 20);
    forward_propagation2(batch.X, cnn, &layers, m);
    backward_propagation2(batch.X, Y_one_hot, cnn, &dcnn, &layers, m);
    parameter_update(cnn, &dcnn, m);
    get_predictions(layers.A1, predictions, m);
    accuracy = get_accuracy(predictions, batch.Y, m);
    printf("Accuracy: %f\n", ((float)accuracy) / ((float)m));
  }
  free(layers.Z1);
  free(layers.A1);
  // free(layers.Z2);
  // free(layers.A2);
  free(Y_one_hot);
  free(predictions);
  return accuracy;
}

int* inference(const float* X, neural_network_t* cnn, const int set_size) {
  neural_network_layers_t layers;
  int* predictions;
  layers.Z1 = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * set_size);
  layers.A1 = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * set_size);
  layers.Z2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * set_size);
  layers.A2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * set_size);
  forward_propagation(X, cnn, &layers, set_size);
  predictions = malloc(sizeof(int) * set_size);
  get_predictions(layers.A2, predictions, set_size);
  free(layers.Z1);
  free(layers.A1);
  free(layers.Z2);
  free(layers.A2);
  return predictions;
}
