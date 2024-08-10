
#include "uart_fsm.h"

#include <stdlib.h>
#include <wiringSerial.h>

#include "neural_network.h"

int uart_init(void) {
  int handler = serialOpen("/dev/serial0", 115200);

  if (handler == -1) {
    printf("Failed to open UART.\n");
  }

  return handler;
}

void end_uart(int handler) {
  serialClose(handler);
  return;
}

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
      return TRAINING;
    case TRAINING:
      return SEND_TRAINING_RESULT;
    case SEND_TRAINING_RESULT:
      return IDLE;
    case WAIT_INFERENCE_DATA:
      return WAIT_INFERENCE_DATA;
    case INFERENCE:
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
      while (!serialDataAvail(handler))
        ;
      mode = serialGetchar(handler);
      if (mode != TRAIN || mode != INFER) return NONE;
      return mode;
    case WAIT_TRAIN_DATA:
      // Obter o tamanho do data set
      data_size = 0;
      for (int i = 0; i < sizeof(int) / sizeof(char); i++) {
        while (!serialDataAvail(handler))
          ;
        data_size = (data_size << 8) + serialGetchar(handler);
      }
      // Obter o data set
      data = malloc((sizeof(float32_t) * data_size * INPUT_LAYER_SIZE));
      for (int i = 0; i < data_size * INPUT_LAYER_SIZE; i++) {
        while (!serialDataAvail(handler))
          ;
        pixel = serialGetchar(handler);
        data[i] = (float32_t)pixel;
      }
      return data_size;
    case WAIT_TRAIN_RESULT:
      result =
          malloc(sizeof(int) * data_size *
                 INPUT_LAYER_SIZE) for (int i = 0;
                                        i < data_size * INPUT_LAYER_SIZE; i++) {
        while (!serialDataAvail(handler))
          ;
        result[i] = serialGetchar(handler);
      }
      return 0;
    case WAIT_TRAIN_ITERATIONS:
      iterations = 0;
      for (int i = 0; i < sizeof(int) / sizeof(char); i++) {
        while (!serialDataAvail(handler))
          ;
        iterations = (iterations << 8) + serialGetchar(handler);
      }
      return iterations;
    case SEND_TRAINING_RESULT:
      message = malloc(sizeof(int) / sizeof(char));
      for (int i = 0; i < sizeof(int) / sizeof(char); i++) {
        message[i] = ((*result & 0xFF000000) >> 24);
        *result <<= 8;
      }
      serialPuts(handler, message);
      return 0;
    case WAIT_INFERENCE_DATA:
      // Obter o tamanho do data set
      data_size = 0;
      for (int i = 0; i < sizeof(int) / sizeof(char); i++) {
        while (!serialDataAvail(handler))
          ;
        data_size = (data_size << 8) + serialGetchar(handler);
      }
      // Obter o data set
      data = malloc((sizeof(float32_t) * data_size * INPUT_LAYER_SIZE));
      for (int i = 0; i < data_size * INPUT_LAYER_SIZE; i++) {
        while (!serialDataAvail(handler))
          ;
        pixel = serialGetchar(handler);
        data[i] = (float32_t)pixel;
      }
      return data_size;
    case SEND_INFERENCE_RESULT:
      message = malloc(sizeof(char) * result_size);
      for (int i = 0; i < result_size; i++) {
        message[i] = (char)result[i] + 0x30;
      }
      serialPuts(handler, message);
      free(message);
      return 0;
    default:
      return 0;
  }
}
