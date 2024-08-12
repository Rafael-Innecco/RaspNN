
#ifndef CLIENTFSM_H
#define CLIENTFSM_H

#define ITERATIONS 1000
#define MNIST_TRAIN_IMAGES_PATH "MNIST/train-images.idx3-ubyte"
#define MNIST_TRAIN_LABELS_PATH "MNIST/train-labels.idx1-ubyte"
#define MNIST_TEST_IMAGES_PATH "MNIST/t10k-images.idx3-ubyte"
#define MNIST_TEST_LABELS_PATH "MNIST/t10k-labels.idx1-ubyte"

#define STRING_MAX_SIZE 1000
#define HELP_STATE "help"
#define TRAIN_DEFAULT_STATE "train_default"
#define TRAIN_CUSTOM_STATE "train_custom"
#define INFERENCE_DEFAULT_STATE "inference_default"
#define INFERENCE_CUSTOM_STATE "inference_custom"
#define CLOSE_STATE "close"

#endif  // CLIENTFSM_H
