// C library headers
#include "client_fsm.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_fsm.h"
#include "socket_wrapper.h"

int main() {
  int sock = -1;
  int num_elements = 0;
  // Init
  sock = socket_connect(SERVER_IP, SERVER_PORT);

  // Train
  // Set Train Mode
  printf("-------- INICIAR TREINO ---------\n");
  char mode = TRAIN;
  socket_write(sock, &mode, sizeof(char));

  // Send Train Data Set Size
  printf("-------- ENVIANDO TAMANHO DOS DADOS DE TREINO ---------\n");
  int data_set_size = TRAIN_DATA_SET_SIZE;
  socket_write(sock, (char*)&data_set_size, sizeof(int) / sizeof(char));

  // Send Train Data Set
  printf("-------- ENVIANDO DADOS DE TREINO ---------\n");
  FILE* file = fopen("MNIST/train-images.idx3-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  char* file_data = malloc(sizeof(char) * TRAIN_DATA_SET_SIZE * IMAGE_SIZE);
  num_elements =
      fread(file_data, sizeof(char), TRAIN_DATA_SET_SIZE * IMAGE_SIZE, file);
  if (num_elements != TRAIN_DATA_SET_SIZE * IMAGE_SIZE) {
    perror("Error writing to file");
  }

  socket_write(sock, file_data, TRAIN_DATA_SET_SIZE * IMAGE_SIZE);
  fclose(file);
  free(file_data);
  // Send Label Data Set
  printf("-------- ENVIANDO RÓTULOS DOS DADOS DE TREINO ---------\n");
  file = fopen("MNIST/train-labels.idx1-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  file_data = malloc(sizeof(char) * TRAIN_DATA_SET_SIZE);
  num_elements = fread(file_data, sizeof(char), TRAIN_DATA_SET_SIZE, file);
  if (num_elements != TRAIN_DATA_SET_SIZE) {
    perror("Error writing to file");
  }

  socket_write(sock, file_data, TRAIN_DATA_SET_SIZE);
  fclose(file);
  free(file_data);
  // Send Number of Iterations
  printf("-------- ENVIANDO NÚMERO DE ITERAÇÕES ---------\n");
  int iterations = ITERATIONS;
  socket_write(sock, (char*)&iterations, sizeof(int) / sizeof(char));
  // Wait for Training Accuracy
  printf("-------- OBTENDO ACURÁCIA ---------\n");
  int accuracy;
  socket_read(sock, (char*)&accuracy, sizeof(int) / sizeof(char));
  printf("Training Accuracy: %d\n", accuracy);

  // Inference
  // Set Inference Mode
  printf("-------- INICIAR INFERÊNCIA ---------\n");
  mode = INFER;
  socket_write(sock, &mode, sizeof(char));

  // Send Inference Data Set Size
  printf("-------- ENVIANDO CONJUNTO DE DADOS PARA INFERENCIA ---------\n");
  data_set_size = INFERENCE_DATA_SET_SIZE;
  socket_write(sock, (char*)&data_set_size, sizeof(int) / sizeof(char));
  // Send Inference Data Set
  file = fopen("MNIST/t10k-images.idx3-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  file_data = malloc(sizeof(char) * INFERENCE_DATA_SET_SIZE * IMAGE_SIZE);
  num_elements = fread(file_data, sizeof(char),
                       INFERENCE_DATA_SET_SIZE * IMAGE_SIZE, file);
  if (num_elements != INFERENCE_DATA_SET_SIZE * IMAGE_SIZE) {
    perror("Error writing to file");
  }
  socket_write(sock, file_data, data_set_size * IMAGE_SIZE);

  fclose(file);
  free(file_data);

  // Wait for Inference Result
  printf("-------- OBTENDO RESULTADO DA INFERENCIA ---------\n");
  char* inference_result = malloc(sizeof(char) * INFERENCE_DATA_SET_SIZE);
  socket_read(sock, inference_result, sizeof(char) * INFERENCE_DATA_SET_SIZE);
  // Compare Inference Result With Expected Result
  file = fopen("MNIST/t10k-labels.idx1-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  file_data = malloc(sizeof(char) * INFERENCE_DATA_SET_SIZE);
  num_elements = fread(file_data, sizeof(char), INFERENCE_DATA_SET_SIZE, file);
  if (num_elements != INFERENCE_DATA_SET_SIZE) {
    perror("Error writing to file");
  }

  fclose(file);
  int accuracy_ = 0;
  for (int i = 0; i < sizeof(char) * INFERENCE_DATA_SET_SIZE; i++) {
    if (file_data[i] == inference_result[i]) accuracy_++;
  }
  printf("Inference Accuracy: %d\n", accuracy_);
  free(file_data);
  free(inference_result);
  // Close
  printf("-------- FECHANDO COMUNICAÇÃO ---------\n");
  mode = CLOSE;
  socket_write(sock, &mode, sizeof(char));
};