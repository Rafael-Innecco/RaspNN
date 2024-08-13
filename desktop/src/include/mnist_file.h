
#ifndef MNISTFILE_H
#define MNISTFILE_H

#include <stdint.h>

#define MNIST_LABEL_MAGIC 0x00000801
#define MNIST_IMAGE_MAGIC 0x00000803
#define MNIST_IMAGE_WIDTH 28
#define MNIST_IMAGE_HEIGHT 28

typedef struct mnist_label_file_header_t_ {
  uint32_t magic_number;
  uint32_t number_of_labels;
} __attribute__((packed)) mnist_label_file_header_t;

typedef struct mnist_image_file_header_t_ {
  uint32_t magic_number;
  uint32_t number_of_images;
  uint32_t number_of_rows;
  uint32_t number_of_columns;
} __attribute__((packed)) mnist_image_file_header_t;

int get_mnist_images(const char* image_path, char** images);
int get_mnist_labels(const char* label_path, char** labels);

#endif  // MNISTFILE_H