// C library headers
#include "client_cli.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mnist_file.h"
#include "socket_wrapper.h"

int train(int sock, const char *images_path, const char *labels_path) {
  char mode;
  char *images, *labels;

  int data_set_size = get_mnist_images(images_path, &images);
  if ((data_set_size != get_mnist_labels(labels_path, &labels)) ||
      (data_set_size == 0)) {
    printf("Erro ao ler arquivos de treino!\n");
    return -1;
  }

  // Set Train Mode
  printf("-------- INICIAR TREINO ---------\n");
  mode = TRAIN;
  socket_write(sock, &mode, sizeof(char));

  // Send Train Data Set Size
  printf("-------- ENVIANDO TAMANHO DOS DADOS DE TREINO ---------\n");
  socket_write(sock, (char *)&data_set_size, sizeof(int) / sizeof(char));

  // Send Train Data Set
  printf("-------- ENVIANDO DADOS DE TREINO ---------\n");
  socket_write(sock, images, data_set_size * IMAGE_SIZE);
  free(images);

  // Send Label Data Set
  printf("-------- ENVIANDO RÓTULOS DOS DADOS DE TREINO ---------\n");
  socket_write(sock, labels, data_set_size);
  free(labels);

  // Send Number of Iterations
  printf("-------- ENVIANDO NÚMERO DE ITERAÇÕES ---------\n");
  int iterations = ITERATIONS;
  socket_write(sock, (char *)&iterations, sizeof(int) / sizeof(char));

  // Wait for Training Accuracy
  printf("-------- OBTENDO ACURÁCIA ---------\n");
  int accuracy;
  socket_read(sock, (char *)&accuracy, sizeof(int) / sizeof(char));
  printf("Training Accuracy: %d\n", accuracy);
  return 0;
}

int inference(int sock, const char *images_path, const char *labels_path,
              int check_predictions) {
  char mode;
  char *images, *labels;

  int data_set_size = get_mnist_images(images_path, &images);
  if ((data_set_size != get_mnist_labels(labels_path, &labels)) ||
      (data_set_size == 0)) {
    printf("Erro ao ler arquivos de inferencia!\n");
    return -1;
  }

  printf("-------- INICIAR INFERÊNCIA ---------\n");
  mode = INFER;
  socket_write(sock, &mode, sizeof(char));

  // Send Inference Data Set Size
  printf("-------- ENVIANDO CONJUNTO DE DADOS PARA INFERENCIA ---------\n");
  socket_write(sock, (char *)&data_set_size, sizeof(int) / sizeof(char));

  // Send Inference Data Set
  socket_write(sock, images, data_set_size * IMAGE_SIZE);
  free(images);

  // Wait for Inference Result
  printf("-------- OBTENDO RESULTADO DA INFERENCIA ---------\n");
  char *inference_result = malloc(sizeof(char) * data_set_size);
  socket_read(sock, inference_result, sizeof(char) * data_set_size);
  if (check_predictions ==
      1) {  // Compare Inference Result With Expected Result
    int accuracy_ = 0;
    for (int i = 0; i < sizeof(char) * data_set_size; i++) {
      if (labels[i] == inference_result[i]) accuracy_++;
    }
    printf("Inference Accuracy: %d\n", accuracy_);
  } else {  // Print Predictions
    for (int i = 0; i < sizeof(char) * data_set_size; i++) {
      printf("%d,", inference_result[i]);
    }
  }

  free(labels);
  free(inference_result);
  return 0;
}

int close_connection(int sock) {
  printf("-------- FECHANDO COMUNICAÇÃO ---------\n");
  char mode = CLOSE;
  socket_write(sock, &mode, sizeof(char));
  return 1;
}

int help(void) {
  printf(
      "Digite o Modo de Operacao Desejado:\n"
      "-- help: Exibe informacoes sobre o funcionamento desse programa\n"
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
  int sock = socket_connect(SERVER_IP, SERVER_PORT);
  char command[STRING_MAX_SIZE] = HELP_COMMAND;
  char images_path[STRING_MAX_SIZE], labels_path[STRING_MAX_SIZE];
  int result = 0, scanf_result;
  result = help();
  while (result != 1) {
    scanf_result = scanf("%s", command);
    if (strcmp(command, HELP_COMMAND) == 0) {
      result = help();
    } else if (strcmp(command, TRAIN_DEFAULT_COMMAND) == 0) {
      result = train(sock, MNIST_TRAIN_IMAGES_PATH, MNIST_TRAIN_LABELS_PATH);
    } else if (strcmp(command, TRAIN_CUSTOM_COMMAND) == 0) {
      scanf_result = scanf("%s", images_path);
      scanf_result = scanf("%s", labels_path);
      result = train(sock, images_path, labels_path);
    } else if (strcmp(command, TEST_DEFAULT_COMMAND) == 0) {
      result =
          inference(sock, MNIST_TEST_IMAGES_PATH, MNIST_TEST_LABELS_PATH, 1);
    } else if (strcmp(command, TEST_CUSTOM_COMMAND) == 0) {
      scanf_result = scanf("%s", images_path);
      scanf_result = scanf("%s", labels_path);
      result = inference(sock, images_path, labels_path, 1);
    } else if (strcmp(command, INFERENCE_COMMAND) == 0) {
      scanf_result = scanf("%s", images_path);
      result = inference(sock, images_path, labels_path, 0);
    } else if (strcmp(command, CLOSE_COMMAND) == 0) {
      result = close_connection(sock);
      return 0;
    } else {
      result = help();
    }
    printf("#");
  }
  printf("Encerrando a Aplicação!\n");
  return 0;
};
