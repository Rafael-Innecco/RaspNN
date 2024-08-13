#include <arm_neon.h>

uint32x4_t vbslq_test(uint32x4_t a, uint32x4_t b, uint32x4_t c) {
  uint32x4_t d = vbslq_u32(a, b, c);
  uint32x4_t e = vaddq_u32(d, a);
  uint32x4_t f = vaddq_u32(d, b);
  uint32x4_t g = vaddq_u32(d, c);
  uint32x4_t h = vbslq_u32(e, f, g);
  return h;
}
