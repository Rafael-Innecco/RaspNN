#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <arm_neon.h>

#define INPUT_LAYER_SIZE 784
#define HIDDEN_LAYER1_SIZE 10
#define HIDDEN_LAYER2_SIZE 10
#define OUTPUT_LAYER_SIZE 10

#define BATCH_SIZE 100
#define LEARNING_RATE 0.1

typedef struct neural_network_t_ {
  float32_t b1[HIDDEN_LAYER1_SIZE];
  float32_t W1[HIDDEN_LAYER1_SIZE * INPUT_LAYER_SIZE];
  float32_t b2[OUTPUT_LAYER_SIZE];
  float32_t W2[OUTPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE];
} neural_network_t;

typedef struct neural_network_layers_t_ {
  float32_t* Z1;
  float32_t* A1;
  float32_t* Z2;
  float32_t* A2;
} neural_network_layers_t;

typedef struct neural_network_gradient_t_ {
  float32_t db1[HIDDEN_LAYER1_SIZE];
  float32_t dW1[HIDDEN_LAYER1_SIZE * INPUT_LAYER_SIZE];
  float32_t db2[OUTPUT_LAYER_SIZE];
  float32_t dW2[OUTPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE];
} neural_network_gradient_t;

typedef struct batch_t_ {
  float32_t* X;
  int* Y;
} batch_t;


int train(float32_t* X, int* Y, neural_network_t* cnn, const int set_size,
          const int iterations);

int* inference(const float32_t* X, neural_network_t* cnn, const int set_size);

#endif  // NEURALNETWORK_H
