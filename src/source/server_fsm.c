
#include "server_fsm.h"

#include <arm_neon.h>
#include <stdlib.h>

#include "socket_wrapper.h"

int next_state(int state, int mode) {
  switch (state) {
    case INIT:
      return IDLE;
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
    case END_CONNECTION:
      return INIT;
    default:
      return IDLE;
  }
}

int action(int state, int sock, int* result, int result_size, float32_t* data) {
  int data_size;
  char* message;
  switch (state) {
    case INIT:
      int sock_ = -1;
      while (sock_ == -1) {
        sock_ = socket_listen(SERVER_PORT, result);
      }
      return sock_;
    case IDLE:
      int mode = 0;
      socket_read(sock, (char*)&mode, sizeof(char));
      if (mode != TRAIN || mode != INFER || mode != CLOSE) return NONE;
      return mode;
    case WAIT_TRAIN_DATA:
      // Obter o tamanho do data set
      socket_read(sock, (char*)&data_size, sizeof(int) / sizeof(char));
      // Obter o data set
      message = malloc((sizeof(char) * data_size * IMAGE_SIZE));
      socket_read(sock, message, data_size * IMAGE_SIZE);
      data = malloc((sizeof(float32_t) * data_size * IMAGE_SIZE));
      for (int i = 0; i < data_size * IMAGE_SIZE; i++) {
        data[i] = (float32_t)message[i];
      }
      free(message);
      return data_size;
    case WAIT_TRAIN_RESULT:
      message = malloc((sizeof(char) * data_size * IMAGE_SIZE));
      socket_read(sock, message, data_size * IMAGE_SIZE);
      result = malloc(sizeof(int) * data_size * IMAGE_SIZE);
      for (int i = 0; i < data_size * IMAGE_SIZE; i++) {
        result[i] = (int)message[i];
      }
      free(message);
      return 0;
    case WAIT_TRAIN_ITERATIONS:
      int iterations;
      socket_read(sock, (char*)&iterations, sizeof(int) / sizeof(char));
      return iterations;
    case SEND_TRAINING_RESULT:
      socket_write(sock, (char*)result, sizeof(int) / sizeof(char));
      return 0;
    case WAIT_INFERENCE_DATA:
      // Obter o tamanho do data set
      socket_read(sock, (char*)&data_size, sizeof(int) / sizeof(char));
      // Obter o data set
      message = malloc((sizeof(char) * data_size * IMAGE_SIZE));
      socket_read(sock, message, data_size * IMAGE_SIZE);
      data = malloc((sizeof(float32_t) * data_size * IMAGE_SIZE));
      for (int i = 0; i < data_size * IMAGE_SIZE; i++) {
        data[i] = (float32_t)message[i];
      }
      free(message);
      return data_size;
    case SEND_INFERENCE_RESULT:
      message = malloc(sizeof(char) * result_size);
      for (int i = 0; i < result_size; i++) {
        message[i] = (char)result[i];
      }
      socket_write(sock, message, result_size);
      free(message);
      return 0;
    case END_CONNECTION:
      socket_close(sock);
    default:
      return 0;
  }
}