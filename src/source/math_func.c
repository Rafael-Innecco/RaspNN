
#include "math_func.h"

static const float32x4_t ones = {1.0, 1.0, 1.0, 1.0};

float32x4_t relu(const float32x4_t A) {
  float32x4_t zeros = vmovq_n_f32(0.0);
  return vbslq_f32(vcgtq_f32(A, zeros), A, zeros);
}

float32x4_t relu_derivate(const float32x4_t A) {
  float32x4_t zeros = vmovq_n_f32(0.0);
  float32x4_t ones = vmovq_n_f32(1.0);
  return vbslq_f32(vcgtq_f32(A, zeros), ones, zeros);
}

// Talvez desistir desse cara?
float32x4_t pow32x4(float32x4_t x, float32x4_t y) {
  float32x4_t z, ax, z_h, z_l, p_h, p_l;
  float32x4_t y1, t1, t2, r, s, t, u, v, w;
  int32x4_t i, j, k, yisint, n;
  int32x4_t hx, hy, ix, iy, is;

  hx = vreinterpretq_s32_f32(x);
  hy = vreinterpretq_s32_f32(y);

  int32x4_t mantissa = vmovq_n_s32(0x7fffffff);
  ix = vandq_s32(hx, mantissa);
  iy = vandq_s32(hy, mantissa);

  return x;
}
