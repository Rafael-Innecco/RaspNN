
#include "server_fsm.h"

#include <stdlib.h>
#include <stdio.h>

#include "socket_wrapper.h"

static struct sockaddr_in address;

int next_state(int state, int mode) {
  switch (state) {
    case INIT:
      return LISTEN;
    case LISTEN:
      return IDLE;
    case IDLE:
      if (mode == TRAIN)
        return WAIT_TRAIN_DATA;
      else if (mode == INFER)
        return WAIT_INFERENCE_DATA;
      else if (mode == CLOSE)
        return END_CONNECTION;
      else
        return IDLE;
    case WAIT_TRAIN_DATA:
      return WAIT_TRAIN_LABELS;
    case WAIT_TRAIN_LABELS:
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
      return LISTEN;
    default:
      return IDLE;
  }
}

int action(int state, int sock, int* result, int size, float32_t** data) {
  int data_size;
  char* message;
  switch (state) {
    case INIT:
      printf("\033[32m");
      printf("------------ INICIANDO SERVIDOR -------------\n");
      printf("\033[0m");
      int server_fd = -1;
      server_fd = socket_server_init(SERVER_PORT, &address);
      return server_fd;
    case LISTEN:
      printf("\033[34m");
      printf("------------ AGUARDANDO CONEXÃO -------------\n");
      printf("\033[0m");
      int sock_ = -1;
      while (sock_ == -1) sock_ = socket_listen(SERVER_PORT, *result, &address);
      printf("\033[36m");
      printf("----------- CONEXÃO BEM SUCEDIDA ------------\n");
      printf("\033[0m");
      return sock_;
    case IDLE:
      char mode;
      socket_read(sock, &mode, sizeof(char));
      if (mode != TRAIN && mode != INFER && mode != CLOSE) return NONE;
      return (int)mode;
    case WAIT_TRAIN_DATA:
      printf("\033[33m");
      printf("------- AGUARDANDO DADOS PARA TREINO --------\n");
      printf("\033[0m");
      // Obter o tamanho do data set
      socket_read(sock, (char*)&data_size, sizeof(int) / sizeof(char));
      // Obter o data set
      message = malloc((sizeof(char) * data_size * IMAGE_SIZE));
      socket_read(sock, message, data_size * IMAGE_SIZE);
      *data = malloc((sizeof(float32_t) * data_size * IMAGE_SIZE));
      for (int i = 0; i < data_size * IMAGE_SIZE; i++) {
        (*data)[i] = (float32_t)message[i] / ((float32_t)255.0);
      }
      free(message);
      return data_size;
    case WAIT_TRAIN_LABELS:
      message = malloc((sizeof(char) * size));
      socket_read(sock, message, size);
      for (int i = 0; i < size; i++) {
        result[i] = (int)message[i];
      }
      free(message);
      return 0;
    case WAIT_TRAIN_ITERATIONS:
      int iterations;
      socket_read(sock, (char*)&iterations, sizeof(int) / sizeof(char));
      return iterations;
    case SEND_TRAINING_RESULT:
      printf("\033[32m");
      printf("------------ ENVIANDO RESULTADOS ------------\n");
      printf("\033[0m");
      socket_write(sock, (char*)(*data), sizeof(int) / sizeof(char));
      return 0;
    case WAIT_INFERENCE_DATA:
      printf("\033[33m");
      printf("----- AGUARDANDO DADOS PARA INFERÊNCIA ------\n");
      printf("\033[0m");
      // Obter o tamanho do data set
      socket_read(sock, (char*)&data_size, sizeof(int) / sizeof(char));
      // Obter o data set
      message = malloc((sizeof(char) * data_size * IMAGE_SIZE));
      socket_read(sock, message, data_size * IMAGE_SIZE);
      *data = malloc((sizeof(float32_t) * data_size * IMAGE_SIZE));
      for (int i = 0; i < data_size * IMAGE_SIZE; i++) {
        (*data)[i] = (float32_t) message[i] / ((float32_t)255.0);
      }
      free(message);
      return data_size;
    case SEND_INFERENCE_RESULT:
      printf("\033[32m");
      printf("------------ ENVIANDO RESULTADOS ------------\n");
      printf("\033[0m");
      message = malloc(sizeof(char) * size);
      for (int i = 0; i < size; i++) {
        message[i] = (char)result[i];
      }
      socket_write(sock, message, size);
      free(message);
      return 0;
    case END_CONNECTION:
      printf("\033[33m");
      printf("---------- ENCERRANDO COMUNICAÇÃO -----------\n");
      printf("\033[0m");
      socket_close(sock);
      return 0;
    default:
      return 0;
  }
}
