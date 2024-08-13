	.arch armv8-a
	.file	"math_func.c"
	.text
	.align	2
	.p2align 4,,11
	.global	relu
	.type	relu, %function
relu:
.LFB4350:
	.cfi_startproc
	fcmgt	v1.4s, v0.4s, 0
	and	v0.16b, v1.16b, v0.16b
	ret
	.cfi_endproc
.LFE4350:
	.size	relu, .-relu
	.align	2
	.p2align 4,,11
	.global	relu_derivate
	.type	relu_derivate, %function
relu_derivate:
.LFB4351:
	.cfi_startproc
	fmov	v1.4s, 1.0e+0
	fcmgt	v0.4s, v0.4s, 0
	and	v0.16b, v0.16b, v1.16b
	ret
	.cfi_endproc
.LFE4351:
	.size	relu_derivate, .-relu_derivate
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
