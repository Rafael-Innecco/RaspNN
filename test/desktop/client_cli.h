
#ifndef CLIENTCLI_H
#define CLIENTCLI_H

#define ITERATIONS 5
#define MNIST_TRAIN_IMAGES_PATH "MNIST/train-images-idx3-ubyte"
#define MNIST_TRAIN_LABELS_PATH "MNIST/train-labels-idx1-ubyte"
#define MNIST_TEST_IMAGES_PATH "MNIST/t10k-images-idx3-ubyte"
#define MNIST_TEST_LABELS_PATH "MNIST/t10k-labels-idx1-ubyte"

#define STRING_MAX_SIZE 1000
#define HELP_COMMAND "help"
#define CONNECT_COMMAND "connect"
#define TRAIN_DEFAULT_COMMAND "train_default"
#define TRAIN_CUSTOM_COMMAND "train_custom"
#define TEST_DEFAULT_COMMAND "test_default"
#define TEST_CUSTOM_COMMAND "test_custom"
#define INFERENCE_COMMAND "inference"
#define CLOSE_COMMAND "close"

#define NONE 0
#define TRAIN 1
#define INFER 2
#define CLOSE 3

#define SERVER_IP "127.0.0.0"
#define SERVER_PORT 8080

#define IMAGE_SIZE 784

#endif  // CLIENTCLI_H
