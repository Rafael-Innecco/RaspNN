	.arch armv8-a
	.file	"vbslq_example.c"
	.text
	.align	2
	.global	vbslq_test
	.type	vbslq_test, %function
vbslq_test:
.LFB4350:
	.cfi_startproc
	sub	sp, sp, #320
	.cfi_def_cfa_offset 320
	str	q0, [sp, 32]
	str	q1, [sp, 16]
	str	q2, [sp]
	ldr	q0, [sp, 32]
	str	q0, [sp, 80]
	ldr	q0, [sp, 16]
	str	q0, [sp, 64]
	ldr	q0, [sp]
	str	q0, [sp, 48]
	ldr	q1, [sp, 80]
	ldr	q2, [sp, 64]
	ldr	q0, [sp, 48]
	bit	v0.16b, v2.16b, v1.16b
	str	q0, [sp, 304]
	ldr	q0, [sp, 304]
	str	q0, [sp, 112]
	ldr	q0, [sp, 32]
	str	q0, [sp, 96]
	ldr	q1, [sp, 112]
	ldr	q0, [sp, 96]
	add	v0.4s, v1.4s, v0.4s
	str	q0, [sp, 288]
	ldr	q0, [sp, 304]
	str	q0, [sp, 144]
	ldr	q0, [sp, 16]
	str	q0, [sp, 128]
	ldr	q1, [sp, 144]
	ldr	q0, [sp, 128]
	add	v0.4s, v1.4s, v0.4s
	str	q0, [sp, 272]
	ldr	q0, [sp, 304]
	str	q0, [sp, 176]
	ldr	q0, [sp]
	str	q0, [sp, 160]
	ldr	q1, [sp, 176]
	ldr	q0, [sp, 160]
	add	v0.4s, v1.4s, v0.4s
	str	q0, [sp, 256]
	ldr	q0, [sp, 288]
	str	q0, [sp, 224]
	ldr	q0, [sp, 272]
	str	q0, [sp, 208]
	ldr	q0, [sp, 256]
	str	q0, [sp, 192]
	ldr	q1, [sp, 224]
	ldr	q2, [sp, 208]
	ldr	q0, [sp, 192]
	bit	v0.16b, v2.16b, v1.16b
	str	q0, [sp, 240]
	ldr	q0, [sp, 240]
	add	sp, sp, 320
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE4350:
	.size	vbslq_test, .-vbslq_test
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
