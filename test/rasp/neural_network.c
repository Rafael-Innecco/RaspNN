#include "neural_network.h"

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void random_params(neural_network_t* cnn) {
  init_matrix(cnn->W1, 0.5, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE);
  init_matrix(cnn->b1, 0.5, HIDDEN_LAYER1_SIZE, 1);
  init_matrix(cnn->W2, 0.5, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  init_matrix(cnn->b2, 0.5, OUTPUT_LAYER_SIZE, 1);
  return;
}

void forward_propagation(const float* X, neural_network_t* cnn,
                         neural_network_layers_t* layers, int m) {
  float* M;
  // input layer
  M = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  multiply_matrix_matrix(X, cnn->W1, M, m, INPUT_LAYER_SIZE,
                         HIDDEN_LAYER1_SIZE);
  sum_matrix_vector(M, cnn->b1, layers->Z1, m, HIDDEN_LAYER1_SIZE);
  relu_matrix(layers->Z1, layers->A1, m, HIDDEN_LAYER1_SIZE);
  free(M);
  // hidden layer 1
  M = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  multiply_matrix_matrix(layers->A1, cnn->W2, M, m, HIDDEN_LAYER1_SIZE,
                         OUTPUT_LAYER_SIZE);
  sum_matrix_vector(M, cnn->b2, layers->Z2, m, OUTPUT_LAYER_SIZE);
  softmax_matrix(layers->Z2, layers->A2, m, OUTPUT_LAYER_SIZE);
  free(M);
  return;
}

void forward_propagation2(const float* X, neural_network_t* cnn,
                          neural_network_layers_t* layers, int m) {
  float* M;
  // input layer
  M = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  // transpose_matrix(X, T, INPUT_LAYER_SIZE, m);
  multiply_matrix_matrix(X, cnn->W1, M, m, INPUT_LAYER_SIZE, OUTPUT_LAYER_SIZE);
  sum_matrix_vector(M, cnn->b1, layers->Z1, m, OUTPUT_LAYER_SIZE);
  softmax_matrix(layers->Z1, layers->A1, m, OUTPUT_LAYER_SIZE);
  free(M);
  return;
}

void backward_propagation(const float* X, const float* Y, neural_network_t* cnn,
                          neural_network_gradient_t* dcnn,
                          neural_network_layers_t* layers, int m) {
  float *dZ1, *dZ2, *M, *relu, *WT, *dZT, *AT;
  // dZ2
  dZ2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  // copy_vector(layers->A2, dZ2, m * OUTPUT_LAYER_SIZE);
  diff_matrix(layers->A2, Y, dZ2, m, OUTPUT_LAYER_SIZE);
  //   dW2
  dZT = malloc((sizeof(float) * OUTPUT_LAYER_SIZE * m));
  transpose_matrix(dZ2, dZT, m, OUTPUT_LAYER_SIZE);
  AT = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  transpose_matrix(layers->A1, AT, m, HIDDEN_LAYER1_SIZE);
  multiply_matrix_matrix(dZT, AT, dcnn->dW2, OUTPUT_LAYER_SIZE, m,
                         HIDDEN_LAYER1_SIZE);
  // dB2
  matrix_redux_float(dZT, dcnn->db2, OUTPUT_LAYER_SIZE, m);
  free(AT);
  free(dZT);
  // dZ1
  WT = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE);
  transpose_matrix(cnn->W2, WT, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  M = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  multiply_matrix_matrix(dZ2, WT, M, m, OUTPUT_LAYER_SIZE, HIDDEN_LAYER1_SIZE);
  relu = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  relu_derivate_matrix(layers->Z1, relu, m, HIDDEN_LAYER1_SIZE);
  dZ1 = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  multiply_matrix_hadamard(M, relu, dZ1, m, HIDDEN_LAYER1_SIZE);
  // dW1
  AT = malloc(sizeof(float) * INPUT_LAYER_SIZE * m);
  transpose_matrix(X, AT, m, INPUT_LAYER_SIZE);
  dZT = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * m);
  transpose_matrix(dZ1, dZT, m, HIDDEN_LAYER1_SIZE);
  multiply_matrix_matrix(dZT, AT, dcnn->dW1, HIDDEN_LAYER1_SIZE, m,
                         INPUT_LAYER_SIZE);
  // print_matrix(dcnn->dW1, HIDDEN_LAYER1_SIZE, INPUT_LAYER_SIZE);
  //  for (int i = 0; i < m; i++) {
  //    printf("Linha: %d\n", i);
  //    for (int j = 0; j < HIDDEN_LAYER1_SIZE; j++) {
  //      printf("Coluna: %d; Valor: %1.3f\n", j, dZT[HIDDEN_LAYER1_SIZE * i +
  //      j]);
  //    }
  //  }
  for (int i = 0; i < HIDDEN_LAYER1_SIZE; i++) {
    printf("Linha: %d\n", i);
    for (int j = 0; j < OUTPUT_LAYER_SIZE; j++) {
      printf("Coluna: %d; Valor: %1.3f\n", j, WT[HIDDEN_LAYER1_SIZE * i + j]);
    }
  }
  // dB1
  matrix_redux_float(dZT, dcnn->db1, HIDDEN_LAYER1_SIZE, m);
  // for (int j = 0; j < HIDDEN_LAYER1_SIZE; j++) {
  //   printf("Linha: %d; Valor: %1.3f\n", j, dcnn->db1[j]);
  // }
  free(WT);
  free(relu);
  free(M);
  free(AT);
  free(dZT);
  free(dZ2);
  free(dZ1);
  return;
}

void backward_propagation2(const float* X, const float* Y,
                           neural_network_t* cnn,
                           neural_network_gradient_t* dcnn,
                           neural_network_layers_t* layers, int m) {
  float *dZ, *dZT, *XT;
  // Calculo dos dZ's
  dZ = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  diff_matrix(layers->A1, Y, dZ, m, OUTPUT_LAYER_SIZE);
  //    Calculo das saidas
  //    dW1
  dZT = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * m);
  transpose_matrix(dZ, dZT, m, OUTPUT_LAYER_SIZE);
  XT = malloc(sizeof(float) * INPUT_LAYER_SIZE * m);
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
  float rate = ((float)-LEARNING_RATE) / ((float)m);
  printf("------------------ MATRIZ -----------------\n");
  // for (int i = 0; i < OUTPUT_LAYER_SIZE; i++) {
  //   printf("Linha: %d\n", i);
  // for (int j = 0; j < HIDDEN_LAYER1_SIZE; j++) {
  //   printf("Coluna: %d; Valor: %1.3f\n", j, cnn->b1[j]);
  // }
  // }
  sum_multiply_matrix_scalar_fast(cnn->W1, dcnn->dW1, rate, HIDDEN_LAYER1_SIZE,
                                  INPUT_LAYER_SIZE);
  sum_multiply_matrix_scalar_fast(cnn->b1, dcnn->db1, rate, 1,
                                  HIDDEN_LAYER1_SIZE);
  sum_multiply_matrix_scalar_fast(cnn->W2, dcnn->dW2, rate, OUTPUT_LAYER_SIZE,
                                  HIDDEN_LAYER1_SIZE);
  sum_multiply_matrix_scalar_fast(cnn->b2, dcnn->db2, rate, 1,
                                  OUTPUT_LAYER_SIZE);
  return;
}

void get_predictions(const float* output_layer, int* predictions, int m) {
  for (int i = 0; i < m; i++) {
    float max = -1.0 * FLOAT_MAX;
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
  predictions = malloc(sizeof(int) * BATCH_SIZE);
  layers.Z1 = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * BATCH_SIZE);
  layers.A1 = malloc(sizeof(float) * HIDDEN_LAYER1_SIZE * BATCH_SIZE);
  layers.Z2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  layers.A2 = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
  Y_one_hot = malloc(sizeof(float) * OUTPUT_LAYER_SIZE * BATCH_SIZE);
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
    get_predictions(layers.A2, predictions, m);
    accuracy = get_accuracy(predictions, batch.Y, m);
    printf("Accuracy: %f\n", ((float)accuracy) / ((float)m));
  }
  free(layers.Z1);
  free(layers.A1);
  free(layers.Z2);
  free(layers.A2);
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
  get_predictions(layers.A1, predictions, set_size);
  free(layers.Z1);
  free(layers.A1);
  free(layers.Z2);
  free(layers.A2);
  return predictions;
}
