#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <arm_neon.h>

#define INPUT_LAYER_SIZE 784
#define HIDDEN_LAYER1_SIZE 1000
#define HIDDEN_LAYER2_SIZE 1000
#define OUTPUT_LAYER_SIZE 10

#define LEARNING_RATE 0.1

// // Vetores de Propagação
// float input_layer[INPUT_LAYER_SIZE*M];
// float input_layer_weight[INPUT_LAYER_SIZE*HIDDEN_LAYER1_SIZE];
// float input_layer_bias[HIDDEN_LAYER1_SIZE];

// float hidden_layer1[HIDDEN_LAYER1_SIZE*M];
// float hidden_layer1_weight[HIDDEN_LAYER1_SIZE*HIDDEN_LAYER2_SIZE];
// float hidden_layer1_bias[HIDDEN_LAYER2_SIZE];

// float hidden_layer2[HIDDEN_LAYER2_SIZE*M];
// float hidden_layer2_weight[HIDDEN_LAYER2_SIZE*OUTPUT_LAYER_SIZE];
// float hidden_layer2_bias[OUTPUT_LAYER_SIZE];

// float output_layer[OUTPUT_LAYER_SIZE*M];

// // Vetores de Propagação Inversa
// float expected_layer[OUTPUT_LAYER_SIZE*M];

// float diff_input_layer_weight[INPUT_LAYER_SIZE*HIDDEN_LAYER1_SIZE];
// float diff_input_layer_bias[HIDDEN_LAYER1_SIZE];

// float diff_hidden_layer1_weight[HIDDEN_LAYER1_SIZE*HIDDEN_LAYER2_SIZE];
// float diff_hidden_layer1_bias[HIDDEN_LAYER2_SIZE];

// float diff_hidden_layer2_weight[HIDDEN_LAYER2_SIZE*OUTPUT_LAYER_SIZE];
// float diff_hidden_layer2_bias[OUTPUT_LAYER_SIZE];

void forward_propagation(float32_t* input_layer, float32_t* input_layer_weight,
                         float32_t* input_layer_bias, float32_t* hidden_layer1,
                         float32_t* hidden_layer1_weight,
                         float32_t* hidden_layer1_bias,
                         float32_t* hidden_layer2,
                         float32_t* hidden_layer2_weight,
                         float32_t* hidden_layer2_bias, float32_t* output_layer,
                         int m);

void backward_propagation(
    float32_t* input_layer, float32_t* input_layer_weight,
    float32_t* input_layer_weight_diff, float32_t* input_layer_bias_diff,
    float32_t* hidden_layer1, float32_t* hidden_layer1_weight,
    float32_t* hidden_layer1_weight_diff, float32_t* hidden_layer1_bias_diff,
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

#endif  // NEURALNETWORK_H