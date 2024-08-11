
#ifndef SERVERFSM_H
#define SERVERFSM_H

#include <arm_neon.h>

// FSM States
#define INIT 0
#define IDLE 1
#define WAIT_TRAIN_DATA 2
#define WAIT_TRAIN_RESULT 3
#define WAIT_TRAIN_ITERATIONS 4
#define SEND_TRAINING_RESULT 5
#define WAIT_INFERENCE_DATA 6
#define SEND_INFERENCE_RESULT 7
#define END_CONNECTION 8

// FSM Modes
#define NONE 0
#define TRAIN 1
#define INFER 2
#define CLOSE 3

#define SERVER_IP "192.168.0.0"
#define SERVER_PORT 8080

#define IMAGE_SIZE 784

int next_state(int state, int mode);

int action(int state, int handler, int* result, int result_size,
           float32_t* data);

#endif  // SERVERFSM_H
