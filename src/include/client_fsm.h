
#ifndef CLIENTFSM_H
#define CLIENTFSM_H

#include <arm_neon.h>

// FSM States
#define IDLE 0
#define WAIT_TRAIN_DATA 1
#define WAIT_TRAIN_RESULT 2
#define WAIT_TRAIN_ITERATIONS 3
#define SEND_TRAINING_RESULT 4
#define WAIT_INFERENCE_DATA 5
#define SEND_INFERENCE_RESULT 6

// FSM Modes
#define NONE 0
#define TRAIN 1
#define INFER 2

#define IMAGE_SIZE 784

int next_state(int state, int mode);

int action(int state, int handler, int* result, int result_size,
           float32_t* data);

#endif  // CLIENTFSM_H
