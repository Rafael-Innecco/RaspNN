
#include "mnist_file.h"

#include <stdio.h>
#include <stdlib.h>

uint32_t to_little_endian(uint32_t in) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return (((in & 0xFF000000) >> 24) | ((in & 0x00FF0000) >> 8) |
          ((in & 0x0000FF00) << 8) | ((in & 0x000000FF) << 24));
#else
  return in;
#endif
}

int get_mnist_images(const char* image_path, uint8_t** images) {
  FILE* file;
  file = fopen(image_path, "rb");
  mnist_image_file_header_t header;

  if (file == NULL) {
    perror("Impossible to open the image file!");
    return 0;
  }

  if (fread(&header, sizeof(mnist_image_file_header_t), 1, file) != 1) {
    perror("Impossible to read the header of the image file!");
    fclose(file);
    return 0;
  }

  header.magic_number = to_little_endian(header.magic_number);
  header.number_of_images = to_little_endian(header.number_of_images);
  header.number_of_rows = to_little_endian(header.number_of_rows);
  header.number_of_columns = to_little_endian(header.number_of_columns);

  if (MNIST_IMAGE_MAGIC != header.magic_number) {
    printf("Invalid magic number for image file header!\n");
    fclose(file);
    return 0;
  }

  if (MNIST_IMAGE_WIDTH != header.number_of_rows) {
    printf("Invalid image width!\n");
    fclose(file);
    return 0;
  }

  if (MNIST_IMAGE_HEIGHT != header.number_of_columns) {
    printf("Invalid image height!\n");
    fclose(file);
    return 0;
  }

  int number_of_images = header.number_of_images;
  // int number_of_images = 60;
  printf("Numero de imagens: %d\n", number_of_images);
  *images = malloc(sizeof(uint8_t) * number_of_images * MNIST_IMAGE_WIDTH *
                   MNIST_IMAGE_HEIGHT);

  if (fread(*images, sizeof(uint8_t),
            number_of_images * MNIST_IMAGE_WIDTH * MNIST_IMAGE_HEIGHT, file) !=
      number_of_images * MNIST_IMAGE_WIDTH * MNIST_IMAGE_HEIGHT) {
    printf("Error while reading images from file!\n");
    free(*images);
    fclose(file);
    return 0;
  }

  fclose(file);

  return number_of_images;
}

int get_mnist_labels(const char* label_path, uint8_t** labels) {
  FILE* file;
  file = fopen(label_path, "rb");
  mnist_label_file_header_t header;

  if (file == NULL) {
    perror("Impossible to open the label file!");
    return 0;
  }

  if (fread(&header, sizeof(mnist_label_file_header_t), 1, file) != 1) {
    perror("Impossible to read the header of the label file!");
    fclose(file);
    return 0;
  }

  header.magic_number = to_little_endian(header.magic_number);

  if (MNIST_LABEL_MAGIC != header.magic_number) {
    printf("Invalid magic number for label file header!\n");
    fclose(file);
    return 0;
  }

  int number_of_labels = to_little_endian(header.number_of_labels);
  // int number_of_labels = 60;
  printf("Numero de labels: %d\n", number_of_labels);
  *labels = malloc(sizeof(uint8_t) * number_of_labels);

  if (fread(*labels, sizeof(uint8_t), number_of_labels, file) !=
      number_of_labels) {
    printf("Error while reading labels from file!\n");
    free(*labels);
    fclose(file);
    return 0;
  }

  fclose(file);

  return number_of_labels;
}
