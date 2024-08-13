#include <arm_neon.h>

float32x4_t vbslq_test(uint32x4_t a, float32x4_t b, float32x4_t c) {
  float32x4_t d = vbslq_f32(a, b, c);
  float32x4_t e = vaddq_f32(d, c);
  return e;
}
