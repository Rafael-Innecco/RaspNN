#include <arm_neon.h>
void matrix_multiply_4x4_neon(float32_t *A, float32_t *B, float32_t *C) {
// these are the columns A
float32x4_t A0;
float32x4_t A1;
float32x4_t A2;
float32x4_t A3;
// these are the columns B
float32x4_t B0;
float32x4_t B1;
float32x4_t B2;
float32x4_t B3;
// these are the columns C
float32x4_t C0;
float32x4_t C1;
float32x4_t C2;
float32x4_t C3;
A0 = vld1q_f32(A);
A1 = vld1q_f32(A+4);
A2 = vld1q_f32(A+8);
A3 = vld1q_f32(A+12);
// Zero accumulators for C values
C0 = vmovq_n_f32(0);
C1 = vmovq_n_f32(0);
C2 = vmovq_n_f32(0);
C3 = vmovq_n_f32(0);
// Multiply accumulate in 4x1 blocks, i.e. each column in C
B0 = vld1q_f32(B);
C0 = vmlaq_f32(C0, A0, B0);
C0 = vmlaq_f32(C0, A1, B0);
C0 = vmlaq_f32(C0, A2, B0);
C0 = vmlaq_f32(C0, A3, B0);
vst1q_f32(C, C0);
B1 = vld1q_f32(B+4);
C1 = vmlaq_f32(C1, A0, B1);
C1 = vmlaq_f32(C1, A1, B1);
C1 = vmlaq_f32(C1, A2, B1);
C1 = vmlaq_f32(C1, A3, B1);
vst1q_f32(C+4, C1);
B2 = vld1q_f32(B+8);
C2 = vmlaq_f32(C2, A0, B2);
C2 = vmlaq_f32(C2, A1, B2);
C2 = vmlaq_f32(C2, A2, B2);
C2 = vmlaq_f32(C2, A3, B2);
vst1q_f32(C+8, C2);
B3 = vld1q_f32(B+12);
C3 = vmlaq_f32(C3, A0, B3);
C3 = vmlaq_f32(C3, A1, B3);
C3 = vmlaq_f32(C3, A2, B3);
C3 = vmlaq_f32(C3, A3, B3);
vst1q_f32(C+12, C3);
}

int main() {
    return 0;
}
