#include "neural_network.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>


// Vetores de Propagação
float input_layer[INPUT_LAYER_SIZE*M];
float input_layer_weight[INPUT_LAYER_SIZE*HIDDEN_LAYER1_SIZE];
float input_layer_bias[HIDDEN_LAYER1_SIZE];

float hidden_layer1[HIDDEN_LAYER1_SIZE*M];
float hidden_layer1_weight[HIDDEN_LAYER1_SIZE*HIDDEN_LAYER2_SIZE];
float hidden_layer1_bias[HIDDEN_LAYER2_SIZE];

float hidden_layer2[HIDDEN_LAYER2_SIZE*M];
float hidden_layer2_weight[HIDDEN_LAYER2_SIZE*OUTPUT_LAYER_SIZE];
float hidden_layer2_bias[OUTPUT_LAYER_SIZE];

float output_layer[OUTPUT_LAYER_SIZE*M];

// Vetores de Propagação Inversa
float expected_layer[OUTPUT_LAYER_SIZE*M];

float diff_input_layer_weight[INPUT_LAYER_SIZE*HIDDEN_LAYER1_SIZE];
float diff_input_layer_bias[HIDDEN_LAYER1_SIZE];

float diff_hidden_layer1_weight[HIDDEN_LAYER1_SIZE*HIDDEN_LAYER2_SIZE];
float diff_hidden_layer1_bias[HIDDEN_LAYER2_SIZE];

float diff_hidden_layer2_weight[HIDDEN_LAYER2_SIZE*OUTPUT_LAYER_SIZE];
float diff_hidden_layer2_bias[OUTPUT_LAYER_SIZE];

void random_init(){ //cria func. de incializacao aleatoria das camadas

    return;
}

void forward_propagation(){
    float *Z1, *Z2;
    //input layer->layer M
    Z1= multiply_matrix_matrix(input_layer_weight,input_layer,HIDDEN_LAYER1_SIZE,INPUT_LAYER_SIZE, M); // Z1 = W*A
    Z2= sum_matrix(Z1,input_layer_bias,HIDDEN_LAYER1_SIZE, M); // Z2 = W*A + B
    hidden_layer1 = relu_matrix(Z2, HIDDEN_LAYER1_SIZE, M); // Hidden1 = relu(W*A + B);
    free(Z1);
    free(Z2);
    //layer M -> layer 2
    Z1= multiply_matrix_matrix(hidden_layer1_weight,hidden_layer1,HIDDEN_LAYER2_SIZE,HIDDEN_LAYER1_SIZE, M); // Z1 = W*A
    Z2= sum_matrix(Z1,hidden_layer1_bias,HIDDEN_LAYER2_SIZE, M); // Z2 = W*A + B
    hidden_layer2 = relu_matrix(Z2, HIDDEN_LAYER2_SIZE, M);  // Hidden2 = relu(W*A + B);
    free(Z1);
    free(Z2);
    //layer 2 -> output layer
    Z1= multiply_matrix_matrix(hidden_layer2_weight,hidden_layer2,OUTPUT_LAYER_SIZE,HIDDEN_LAYER2_SIZE, M); // Z1 = W*A
    Z2= sum_matrix(Z1,hidden_layer2_bias,OUTPUT_LAYER_SIZE, M); // Z2 = W*A + B
    output_layer = softmax_matrix(Z2, OUTPUT_LAYER_SIZE, M);  // Output = softmax(W*A + B);
    free(Z1);
    free(Z2);
    return;
}

void backward_propagation(){
    float *dZ1, *dZ2, *dZ3;
    dZ3 = sum_matrix(output_layer, multiply_matrix_scalar(expected_layer, -1, OUTPUT_LAYER_SIZE, M), OUTPUT_LAYER_SIZE, M);

    return;
}

