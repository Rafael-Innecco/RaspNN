
#include "client_fsm.h"

#include <stdlib.h>

#include "neural_network.h"
#include "socket_wrapper.h"

int next_state(int state, int mode) {
  switch (state) {
    case IDLE:
      if (mode == TRAIN)
        return WAIT_TRAIN_DATA;
      else if (mode == INFER)
        return WAIT_INFERENCE_DATA;
    case WAIT_TRAIN_DATA:
      return WAIT_TRAIN_DATA;
    case WAIT_TRAIN_RESULT:
      return WAIT_TRAIN_ITERATIONS;
    case WAIT_TRAIN_ITERATIONS:
      return SEND_TRAINING_RESULT;
    case SEND_TRAINING_RESULT:
      return IDLE;
    case WAIT_INFERENCE_DATA:
      return SEND_INFERENCE_RESULT;
    case SEND_INFERENCE_RESULT:
      return IDLE;
    default:
      return IDLE;
  }
}

int action(int state, int handler, int* result, int result_size,
           float32_t* data) {
  int data_size, pixel, iterations, mode;
  char* message;
  switch (state) {
    case IDLE:
      mode = 0;
      socket_read(handler, (char*)&mode, sizeof(char));
      if (mode != TRAIN || mode != INFER) return NONE;
      return mode;
    case WAIT_TRAIN_DATA:
      // Obter o tamanho do data set
      socket_read(handler, (char*)&data_size, sizeof(int) / sizeof(char));
      // Obter o data set
      message = malloc((sizeof(char) * data_size * INPUT_LAYER_SIZE));
      socket_read(handler, message, data_size * INPUT_LAYER_SIZE);
      data = malloc((sizeof(float32_t) * data_size * INPUT_LAYER_SIZE));
      for (int i = 0; i < data_size * INPUT_LAYER_SIZE; i++) {
        data[i] = (float32_t)message[i];
      }
      free(message);
      return data_size;
    case WAIT_TRAIN_RESULT:
      message = malloc((sizeof(char) * data_size * INPUT_LAYER_SIZE));
      socket_read(handler, message, data_size * INPUT_LAYER_SIZE);
      result = malloc(sizeof(int) * data_size * INPUT_LAYER_SIZE);
      for (int i = 0; i < data_size * INPUT_LAYER_SIZE; i++) {
        result[i] = (int)message[i];
      }
      free(message);
      return 0;
    case WAIT_TRAIN_ITERATIONS:
      socket_read(handler, (char*)&iterations, sizeof(int) / sizeof(char));
      return iterations;
    case SEND_TRAINING_RESULT:
      socket_write(handler, (char*)result, sizeof(int) / sizeof(char));
      return 0;
    case WAIT_INFERENCE_DATA:
      // Obter o tamanho do data set
      socket_read(handler, (char*)&data_size, sizeof(int) / sizeof(char));
      // Obter o data set
      message = malloc((sizeof(char) * data_size * INPUT_LAYER_SIZE));
      socket_read(handler, message, data_size * INPUT_LAYER_SIZE);
      data = malloc((sizeof(float32_t) * data_size * INPUT_LAYER_SIZE));
      for (int i = 0; i < data_size * INPUT_LAYER_SIZE; i++) {
        data[i] = (float32_t)message[i];
      }
      free(message);
      return data_size;
    case SEND_INFERENCE_RESULT:
      message = malloc(sizeof(char) * result_size);
      for (int i = 0; i < result_size; i++) {
        message[i] = (char)result[i] + 0x30;
      }
      socket_write(handler, message, result_size);
      free(message);
      return 0;
    default:
      return 0;
  }
}
