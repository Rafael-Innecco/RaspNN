
#include <stdio.h>
#include <stdlib.h>

#include "mnist_file.h"
#include "server_fsm.h"
#include "socket_wrapper.h"

int main() {
  // CNN Variables
  float *data_set;
  int *result, accuracy;
  int data_set_size, iterations;
  char *images, *labels;

  // FSM Variables
  int state, sock, mode, action_result, server_fd;
  size_t num_elements;

  // Arquivos
  FILE *file;

  state = INIT;

  while (1) {
    printf("State: %d\n", state);
    action_result = action(state, sock, result, data_set_size, &data_set);
    switch (state) {
      case INIT:
        server_fd = action_result;
        result = &server_fd;
        break;
      case LISTEN:
        sock = action_result;
        break;
      case IDLE:
        mode = action_result;
        printf("New Mode: %d\n", mode);
        break;
      case WAIT_TRAIN_DATA:
        data_set_size = action_result;
        printf("New Train Data Set Size: %d\n", data_set_size);
        images = malloc(sizeof(char) * data_set_size * IMAGE_SIZE);
        fflush(stdout);
        for (int i = 0; i < data_set_size * IMAGE_SIZE; i++) {
          images[i] = (char)data_set[i];
        }
        result = malloc(sizeof(int) * data_set_size);
        break;
      case WAIT_TRAIN_LABELS:
        labels = malloc(sizeof(char) * data_set_size);
        for (int i = 0; i < data_set_size; i++) {
          labels[i] = (char)result[i];
        }
        file = fopen("MNIST2/train-labels", "wb");
        if (file == NULL) {
          perror("Error opening file");
          return 1;
        }
        num_elements = fwrite(labels, sizeof(char), data_set_size, file);
        if (num_elements != data_set_size) {
          perror("Error writing to file");
        }
        fclose(file);
        free(result);
        break;
      case WAIT_TRAIN_ITERATIONS:
        iterations = action_result;
        printf("New Iterations: %d\n", iterations);
        file = fopen("MNIST2/train-images", "wb");
        if (file == NULL) {
          perror("Error opening file");
          return 1;
        }
        num_elements =
            fwrite(images, sizeof(char), data_set_size * IMAGE_SIZE, file);
        if (num_elements != data_set_size * IMAGE_SIZE) {
          perror("Error writing to file");
        }
        accuracy = 570;
        result = &accuracy;
        fclose(file);
        free(images);
        free(data_set);
        free(labels);
        break;
      case SEND_TRAINING_RESULT:
        break;
      case WAIT_INFERENCE_DATA:
        data_set_size = action_result;
        printf("New Inference Data Set Size: %d\n", data_set_size);
        images = malloc(sizeof(char) * data_set_size * IMAGE_SIZE);
        for (int i = 0; i < data_set_size * IMAGE_SIZE; i++) {
          images[i] = (char)data_set[i];
        }
        file = fopen("MNIST2/test-images", "wb");
        if (file == NULL) {
          perror("Error opening file");
          return 1;
        }
        num_elements =
            fwrite(images, sizeof(char), data_set_size * IMAGE_SIZE, file);
        if (num_elements != data_set_size * IMAGE_SIZE) {
          perror("Error writing to file");
        }
        fclose(file);
        free(images);
        free(data_set);
        data_set_size = get_mnist_labels("MNIST2/test-labels", &labels);
        result = malloc(sizeof(int) * data_set_size);
        for (int i = 0; i < data_set_size; i++) {
          result[i] = (int)labels[i];
        }
        fclose(file);
        free(labels);
        break;
      case SEND_INFERENCE_RESULT:
        free(result);
        break;
      case END_CONNECTION:
        result = &server_fd;
        break;
      default:
    }
    state = next_state(state, mode);
  }

  return 0;
}