// C library headers
#include "server_fsm.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_fsm.h"
#include "socket_wrapper.h"

int main() {
  int sock = -1;
  int server_handler;
  // Init
  while (sock == -1) {
    sock = socket_listen(HOST_PORT, &server_handler);
  }

  // Train
  // Set Train Mode
  char mode = TRAIN;
  socket_write(sock, &mode, sizeof(char));
  // Send Train Data Set Size
  int data_set_size = TRAIN_DATA_SET_SIZE;
  socket_write(sock, (char*)&data_set_size, sizeof(int) / sizeof(char));
  // Send Train Data Set
  FILE* file = fopen("MNIST/train-images.idx3-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  char* file_data = malloc(sizeof(char) * TRAIN_DATA_SET_SIZE * IMAGE_SIZE);
  while (fgets(file_data, sizeof(char) * TRAIN_DATA_SET_SIZE * IMAGE_SIZE,
               file) != NULL)
    ;
  fclose(file);
  free(file_data);
  // Send Label Data Set
  file = fopen("MNIST/train-labels.idx1-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  file_data = malloc(sizeof(char) * TRAIN_DATA_SET_SIZE);
  while (fgets(file_data, sizeof(char) * TRAIN_DATA_SET_SIZE, file) != NULL)
    ;
  fclose(file);
  free(file_data);
  // Send Number of Iterations
  int iterations = ITERATIONS;
  socket_write(sock, (char*)&iterations, sizeof(int) / sizeof(char));
  // Wait for Training Accuracy
  int accuracy;
  socket_read(sock, (char*)&accuracy, sizeof(int) / sizeof(char));
  printf("Training Accuracy: %d", accuracy);
  // Send Inference Data Set Size
  data_set_size = INFERENCE_DATA_SET_SIZE;
  socket_write(sock, (char*)&data_set_size, sizeof(int) / sizeof(char));
  // Send Inference Data Set
  file = fopen("MNIST/t10k-images.idx3-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  file_data = malloc(sizeof(char) * INFERENCE_DATA_SET_SIZE * IMAGE_SIZE);
  while (fgets(file_data, sizeof(char) * INFERENCE_DATA_SET_SIZE * IMAGE_SIZE,
               file) != NULL)
    ;
  fclose(file);
  free(file_data);
  // Wait for Inference Result
  char* inference_result = malloc(sizeof(char) * INFERENCE_DATA_SET_SIZE);
  socket_read(sock, inference_result, sizeof(char) * INFERENCE_DATA_SET_SIZE);
  // Compare Inference Result With Expected Result
  file = fopen("MNIST/t10k-labels.idx1-ubyte", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  file_data = malloc(sizeof(char) * INFERENCE_DATA_SET_SIZE);
  while (fgets(file_data, sizeof(char) * INFERENCE_DATA_SET_SIZE, file) != NULL)
    ;
  fclose(file);
  int accuracy_ = 0;
  for (int i = 0; i < sizeof(char) * INFERENCE_DATA_SET_SIZE; i++) {
    if (file_data[i] == inference_result[i]) accuracy_++;
  }
  printf("Inference Accuracy: %d", accuracy);
  free(file_data);
  // Close
  socket_close(sock);
  socket_close(server_handler);
};