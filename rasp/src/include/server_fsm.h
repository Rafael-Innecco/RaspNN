
#ifndef SERVERFSM_H
#define SERVERFSM_H

#include <arm_neon.h>

// FSM States
#define INIT 0
#define LISTEN 1
#define IDLE 2
#define WAIT_TRAIN_DATA 3
#define WAIT_TRAIN_LABELS 4
#define WAIT_TRAIN_ITERATIONS 5
#define SEND_TRAINING_RESULT 6
#define WAIT_INFERENCE_DATA 7
#define SEND_INFERENCE_RESULT 8
#define END_CONNECTION 9

// FSM Modes
#define NONE 0
#define TRAIN 1
#define INFER 2
#define CLOSE 3

#define SERVER_IP "10.42.0.62"
#define SERVER_PORT 8080

#define IMAGE_SIZE 784

int next_state(int state, int mode);

int action(int state, int handler, int* result, int size, float32_t** data);

#endif  // SERVERFSM_H
