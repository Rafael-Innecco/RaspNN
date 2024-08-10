
#include "math_func.h"

float32x4_t relu(const float32x4_t A) {
  float32x4_t zeros = vmovq_n_f32(0.0);
  return vbslq_f32(vcgtq_f32(A, zeros), A, zeros);
}

float32x4_t relu_derivate(const float32x4_t A) {
  float32x4_t zeros = vmovq_n_f32(0.0);
  float32x4_t ones = vmovq_n_f32(1.0);
  return vbslq_f32(vcgtq_f32(A, zeros), ones, zeros);
}
