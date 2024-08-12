#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <arm_neon.h>

#define INPUT_LAYER_SIZE 784
#define HIDDEN_LAYER1_SIZE 100
#define HIDDEN_LAYER2_SIZE 100
#define OUTPUT_LAYER_SIZE 10

#define LEARNING_RATE 0.1

int train(float32_t* train_set, const int* expected_output,
          float32_t* input_layer_weight, float32_t* input_layer_bias,
          float32_t* hidden_layer1_weight, float32_t* hidden_layer1_bias,
          float32_t* hidden_layer2_weight, float32_t* hidden_layer2_bias,
          const int train_set_size, const int iterations);

int* inference(float32_t* input_layer, float32_t* input_layer_weight,
               float32_t* input_layer_bias, float32_t* hidden_layer1_weight,
               float32_t* hidden_layer1_bias, float32_t* hidden_layer2_weight,
               float32_t* hidden_layer2_bias, const int inference_set_size);

#endif  // NEURALNETWORK_H