
#ifndef MATHFUNCNEON_H
#define MATHFUNCNEON_H

#include <arm_neon.h>

float32x4_t relu(const float32x4_t A);

float32x4_t relu_derivate(const float32x4_t A);

float32x4_t exp32x4(float32x4_t A);

void find_min_max(float32_t* A, float32_t* min, float32_t max);

#endif  // MATHFUNCNEON_H
