#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <arm_neon.h>

#define INPUT_LAYER_SIZE 784
#define HIDDEN_LAYER1_SIZE 1000
#define HIDDEN_LAYER2_SIZE 1000
#define OUTPUT_LAYER_SIZE 10

#define LEARNING_RATE 0.1

void random_params(float32_t* input_layer_weight, float32_t* input_layer_bias,
                   float32_t* hidden_layer1_weight,
                   float32_t* hidden_layer1_bias,
                   float32_t* hidden_layer2_weight,
                   float32_t* hidden_layer2_bias);

void forward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_bias, float32_t* input_layer_pre_activation,
    float32_t* hidden_layer1, float32_t* hidden_layer1_weight,
    float32_t* hidden_layer1_bias, float32_t* hidden_layer1_pre_activation,
    float32_t* hidden_layer2, float32_t* hidden_layer2_weight,
    float32_t* hidden_layer2_bias, float32_t* hidden_layer2_pre_activation,
    float32_t* output_layer, int m);

void backward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_weight_diff, float32_t* input_layer_bias_diff,
    float32_t* input_layer_pre_activation, float32_t* hidden_layer1,
    float32_t* hidden_layer1_weight, float32_t* hidden_layer1_weight_diff,
    float32_t* hidden_layer1_bias_diff, float32_t* hidden_layer1_pre_activation,
    float32_t* hidden_layer2, float32_t* hidden_layer2_weight,
    float32_t* hidden_layer2_weight_diff, float32_t* hidden_layer2_bias_diff,
    float32_t* output_layer, float32_t* expected_layer, int m);

void parameter_update(
    float32_t* input_layer_weight, float32_t* input_layer_weight_diff,
    float32_t* input_layer_bias, float32_t* input_layer_bias_diff,
    float32_t* hidden_layer1_weight, float32_t* hidden_layer1_weight_diff,
    float32_t* hidden_layer1_bias, float32_t* hidden_layer1_bias_diff,
    float32_t* hidden_layer2_weight, float32_t* hidden_layer2_weight_diff,
    float32_t* hidden_layer2_bias, float32_t* hidden_layer2_bias_diff);

void train_network(float32_t* train_set, const int* expected_output,
                   float32_t* input_layer_weight, float32_t* input_layer_bias,
                   float32_t* hidden_layer1_weight,
                   float32_t* hidden_layer1_bias,
                   float32_t* hidden_layer2_weight,
                   float32_t* hidden_layer2_bias, const int train_set_size,
                   const int iterations);

int* get_predictions(const float32_t* output_layer, int m);

float32_t get_accuracy(const int* predictions, const int* expected, int m);

#endif  // NEURALNETWORK_H