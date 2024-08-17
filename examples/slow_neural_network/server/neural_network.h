#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#define INPUT_LAYER_SIZE 784
#define HIDDEN_LAYER1_SIZE 10
#define HIDDEN_LAYER2_SIZE 10
#define OUTPUT_LAYER_SIZE 10

#define BATCH_SIZE 100
#define LEARNING_RATE 0.5

typedef struct neural_network_t_ {
  float b1[HIDDEN_LAYER1_SIZE];
  float W1[HIDDEN_LAYER1_SIZE * INPUT_LAYER_SIZE];
  float b2[OUTPUT_LAYER_SIZE];
  float W2[OUTPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE];
} neural_network_t;

typedef struct neural_network_layers_t_ {
  float* Z1;
  float* A1;
  float* Z2;
  float* A2;
} neural_network_layers_t;

typedef struct neural_network_gradient_t_ {
  float db1[HIDDEN_LAYER1_SIZE];
  float dW1[HIDDEN_LAYER1_SIZE * INPUT_LAYER_SIZE];
  float db2[OUTPUT_LAYER_SIZE];
  float dW2[OUTPUT_LAYER_SIZE * HIDDEN_LAYER1_SIZE];
} neural_network_gradient_t;

typedef struct batch_t_ {
  float* X;
  int* Y;
} batch_t;

int train(float* X, int* Y, neural_network_t* cnn, const int set_size,
          const int iterations);

int* inference(const float* X, neural_network_t* cnn, const int set_size);

#endif  // NEURALNETWORK_H