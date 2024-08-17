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

  // Set Train Mode
  printf("-------- INICIAR TREINO ---------\n");
  mode = TRAIN;
  socket_write(sock, &mode, sizeof(uint8_t));

  // Send Train Data Set Size
  printf("-------- ENVIANDO TAMANHO DOS DADOS DE TREINO ---------\n");
  socket_write(sock, (uint8_t *)&data_set_size, sizeof(int) / sizeof(uint8_t));

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
  socket_write(sock, (uint8_t *)&iterations, sizeof(int) / sizeof(uint8_t));

  // Wait for Training Accuracy
  printf("-------- OBTENDO ACURÁCIA ---------\n");
  int accuracy;
  socket_read(sock, (uint8_t *)&accuracy, sizeof(int) / sizeof(uint8_t));
  printf("Training Accuracy: %d%%\n", accuracy);
  return 0;
}

int close_connection(int sock) {
  printf("-------- FECHANDO COMUNICAÇÃO ---------\n");
  uint8_t mode = CLOSE;
  socket_write(sock, &mode, sizeof(uint8_t));
  return 1;
}

int main() {
  int sock = socket_connect(SERVER_IP, SERVER_PORT);
  int result = 0;
  result = train(sock, MNIST_TRAIN_IMAGES_PATH, MNIST_TRAIN_LABELS_PATH);
  close_connection(sock);
  return 0;
};
