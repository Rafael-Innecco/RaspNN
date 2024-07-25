
#ifndef MATHFUNCNEON_H
#define MATHFUNCNEON_H

#include <arm_neon.h>

float32x4_t relu(float32x4_t A);

float32x4_t exp(float32x4_t A);

float32x4_t softmax(float32x4_t A);

#endif // MATHFUNCNEON_H