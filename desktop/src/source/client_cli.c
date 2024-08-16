// C library headers
#include "client_cli.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mnist_file.h"
#include "socket_wrapper.h"

int train(int sock, const char *images_path, const char *labels_path) {
  uint8_t mode;
  uint8_t *images, *labels;

  int data_set_size = get_mnist_images(images_path, &images);
  if ((data_set_size != get_mnist_labels(labels_path, &labels)) ||
      (data_set_size == 0)) {
    printf("Erro ao ler arquivos de treino!\n");
    return -1;
  }

  printf("\033[36m");
  // Set Train Mode
  printf("---------------- INICIAR TREINO -----------------\n");
  mode = TRAIN;
  socket_write(sock, &mode, sizeof(uint8_t));

  printf("\033[33m");
  // Send Train Data Set Size
  printf("----------- ENVIANDO DADOS DE TREINO ------------\n");
  socket_write(sock, (uint8_t *)&data_set_size, sizeof(int) / sizeof(uint8_t));

  // Send Train Data Set
  socket_write(sock, images, data_set_size * IMAGE_SIZE);
  free(images);

  // Send Label Data Set
  socket_write(sock, labels, data_set_size);
  free(labels);
  
  printf("--------- ENVIANDO NÚMERO DE ITERAÇÕES ----------\n");
  // Send Number of Iterations
  int iterations = ITERATIONS;
  socket_write(sock, (uint8_t *)&iterations, sizeof(int) / sizeof(uint8_t));

  printf("------------- AGUARDANDO RESULTADOS -------------\n");
  // Wait for Training Accuracy
  float accuracy;
  socket_read(sock, (uint8_t *)&accuracy, sizeof(int) / sizeof(uint8_t));
  printf("\033[32m");
  printf("------------- RESULTADOS  RECEBIDOS -------------\n");
  printf("Acurácia: %f\033[0m\n", accuracy);
  return 0;
}

int inference(int sock, const char *images_path, const char *labels_path,
              int check_predictions) {
  uint8_t mode;
  uint8_t *images, *labels;

  int data_set_size = get_mnist_images(images_path, &images);
  if (check_predictions == 1 && (data_set_size != get_mnist_labels(labels_path, &labels)) ||
      (data_set_size == 0)) {
    printf("Erro ao ler arquivos de inferencia!\n");
    return -1;
  }

  printf("\033[36m");
  printf("-------------- INICIAR INFERÊNCIA ---------------\n");
  mode = INFER;
  socket_write(sock, &mode, sizeof(uint8_t));

  // Send Inference Data Set Size
  socket_write(sock, (uint8_t *)&data_set_size, sizeof(int) / sizeof(uint8_t));

  printf("\033[33m");
  // Send Inference Data Set
  printf("-------- ENVIANDO DADOS PARA INFERENCIA ---------\n");
  socket_write(sock, images, data_set_size * IMAGE_SIZE);
  free(images);

  printf("------------- AGUARDANDO RESULTADOS -------------\n");
  // Wait for Inference Result
  uint8_t *inference_result = malloc(sizeof(uint8_t) * data_set_size);
  socket_read(sock, inference_result, sizeof(uint8_t) * data_set_size);
  // Compare Inference Result With Expected Result
  printf("\033[32m");
  printf("------------- RESULTADOS  RECEBIDOS -------------\n");
  if (check_predictions == 1) {
    int accuracy_ = 0;
    for (int i = 0; i < sizeof(uint8_t) * data_set_size; i++) {
      if (labels[i] == inference_result[i]) accuracy_++;
    }
    printf("Acurácia: %1.3f\n", (float)accuracy_ / (float)data_set_size);
  free(labels);
  } else {  // Print Predictions
    printf("Predições:\n");
    for (int i = 0; i < sizeof(uint8_t) * data_set_size; i++) {
      printf("%d,", inference_result[i]);
    }
    printf("\n");
  }
  printf("\033[0m");

  free(inference_result);
  return 0;
}

int close_connection(int sock) {
  printf("\033[33m");
  printf("------------ ENCERRANDO COMUNICAÇÃO -------------\n");
  printf("\033[0m");
  uint8_t mode = CLOSE;
  socket_write(sock, &mode, sizeof(uint8_t));
  return 1;
}

int help(void) {
  printf(
      "Digite o Modo de Operacao Desejado:\n"
      "-- help: Exibe informacoes sobre o funcionamento desse programa\n"
      "-- connect <ip> <port>: conecta a um servidor com IPv4 <ip> e porta "
      "<port>\n"
      "-- train_default: Treina com o data set padrao do MNIST\n"
      "-- train_custom <image_path> <label_path>: Treina com o <image_path> e "
      "o <label_path> fornecidos pelo usuario\n"
      "-- test_default: Testa a rede neural com o data set padrao do "
      "MNIST\n"
      "-- test_custom <image_path> <label_path>: Testa a rede neural com o "
      "<image_path> e o <label_path> fornecidos pelo usuario\n"
      "-- inference <image_path>: Realiza a inferencia com um conjunto de "
      "imagens dado em <image_path>\n"
      "-- close: encerra o cliente\n");

  return 0;
}

int main() {
  int sock = -1;
  char command[STRING_MAX_SIZE] = HELP_COMMAND;
  char arg1[STRING_MAX_SIZE], arg2[STRING_MAX_SIZE];
  int result = 0, scanf_result;
  result = help();
  while (result != 1) {
    printf("#");
    scanf_result = scanf("%s", command);
    if (strcmp(command, HELP_COMMAND) == 0) {
      result = help();
    } else if (strcmp(command, CONNECT_COMMAND) == 0) {
      scanf_result = scanf("%s", arg1);
      scanf_result = scanf("%s", arg2);
      sock = socket_connect(arg1, atoi(arg2));
    } else if (strcmp(command, TRAIN_DEFAULT_COMMAND) == 0) {
      result = train(sock, MNIST_TRAIN_IMAGES_PATH, MNIST_TRAIN_LABELS_PATH);
    } else if (strcmp(command, TRAIN_CUSTOM_COMMAND) == 0) {
      scanf_result = scanf("%s", arg1);
      scanf_result = scanf("%s", arg2);
      result = train(sock, arg1, arg2);
    } else if (strcmp(command, TEST_DEFAULT_COMMAND) == 0) {
      result =
          inference(sock, MNIST_TEST_IMAGES_PATH, MNIST_TEST_LABELS_PATH, 1);
    } else if (strcmp(command, TEST_CUSTOM_COMMAND) == 0) {
      scanf_result = scanf("%s", arg1);
      scanf_result = scanf("%s", arg2);
      result = inference(sock, arg1, arg2, 1);
    } else if (strcmp(command, INFERENCE_COMMAND) == 0) {
      scanf_result = scanf("%s", arg1);
      result = inference(sock, arg1, arg2, 0);
    } else if (strcmp(command, CLOSE_COMMAND) == 0) {
      result = close_connection(sock);
      return 0;
    } else {
      result = help();
    }
  }
  printf("Encerrando a Aplicação!\n");
  return 0;
};
