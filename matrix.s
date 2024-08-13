	.arch armv8-a
	.file	"matrix.c"
	.text
	.align	2
	.p2align 4,,11
	.global	init_matrix
	.type	init_matrix, %function
init_matrix:
.LFB4372:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	str	d8, [sp, 32]
	.cfi_offset 72, -16
	fmov	s8, s0
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	w20, w0
	smull	x0, w1, w0
	mov	w19, w1
	lsl	x0, x0, 2
	bl	malloc
	mul	w3, w19, w20
	mov	x19, x0
	negs	w1, w3
	and	w0, w3, 3
	and	w1, w1, 3
	csneg	w1, w0, w1, mi
	sub	w1, w3, w1
	cmp	w1, 0
	ble	.L5
	dup	v0.4s, v8.s[0]
	sub	w2, w1, #1
	mov	x1, x19
	lsr	w2, w2, 2
	add	w2, w2, 1
	add	x4, x19, w2, uxtw 4
	.p2align 3,,7
.L3:
	str	q0, [x1], 16
	cmp	x1, x4
	bne	.L3
	lsl	w1, w2, 2
.L2:
	cmp	w3, w1
	ble	.L1
	sub	w2, w3, #1
	add	x0, x19, w1, sxtw 2
	sub	w2, w2, w1
	mov	w1, 0
	add	x2, x2, 1
	lsl	x2, x2, 2
	bl	memset
.L1:
	ldr	d8, [sp, 32]
	mov	x0, x19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 72
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L5:
	.cfi_restore_state
	mov	w1, 0
	b	.L2
	.cfi_endproc
.LFE4372:
	.size	init_matrix, .-init_matrix
	.align	2
	.p2align 4,,11
	.global	init_matrix_random
	.type	init_matrix_random, %function
init_matrix_random:
.LFB4373:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	w19, w0
	smull	x0, w1, w0
	str	x21, [sp, 32]
	mul	w20, w1, w19
	lsl	x0, x0, 2
	.cfi_offset 21, -16
	bl	malloc
	mov	x21, x0
	mov	x0, 0
	bl	time
	bl	srand
	cmp	w20, 0
	ble	.L9
	str	d8, [sp, 40]
	.cfi_offset 72, -8
	movi	v8.2s, 0x30, lsl 24
	add	x20, x21, w20, sxtw 2
	mov	x19, x21
	.p2align 3,,7
.L11:
	bl	rand
	scvtf	s0, w0
	fmul	s0, s0, s8
	str	s0, [x19], 4
	cmp	x19, x20
	bne	.L11
	ldr	d8, [sp, 40]
	.cfi_restore 72
.L9:
	ldp	x19, x20, [sp, 16]
	mov	x0, x21
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE4373:
	.size	init_matrix_random, .-init_matrix_random
	.align	2
	.p2align 4,,11
	.global	sum_matrix
	.type	sum_matrix, %function
sum_matrix:
.LFB4374:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	x19, x0
	smull	x0, w3, w2
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -16
	.cfi_offset 22, -8
	mov	w22, w2
	mov	w21, w3
	mov	x20, x1
	lsl	x0, x0, 2
	bl	malloc
	mul	w1, w21, w22
	negs	w2, w1
	and	w3, w1, 3
	and	w2, w2, 3
	csneg	w2, w3, w2, mi
	sub	w2, w1, w2
	cmp	w2, 0
	ble	.L19
	sub	w2, w2, #1
	mov	x3, 0
	lsr	w2, w2, 2
	add	w2, w2, 1
	ubfiz	x4, x2, 4, 31
	.p2align 3,,7
.L16:
	ldr	q0, [x19, x3]
	ldr	q1, [x20, x3]
	fadd	v0.4s, v0.4s, v1.4s
	str	q0, [x0, x3]
	add	x3, x3, 16
	cmp	x4, x3
	bne	.L16
	lsl	w2, w2, 2
.L15:
	cmp	w1, w2
	ble	.L14
	sxtw	x2, w2
	.p2align 3,,7
.L18:
	ldr	s0, [x19, x2, lsl 2]
	ldr	s1, [x20, x2, lsl 2]
	fadd	s0, s0, s1
	str	s0, [x0, x2, lsl 2]
	add	x2, x2, 1
	cmp	w1, w2
	bgt	.L18
.L14:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L19:
	.cfi_restore_state
	mov	w2, 0
	b	.L15
	.cfi_endproc
.LFE4374:
	.size	sum_matrix, .-sum_matrix
	.align	2
	.p2align 4,,11
	.global	sum_matrix_vector
	.type	sum_matrix_vector, %function
sum_matrix_vector:
.LFB4375:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -32
	.cfi_offset 22, -24
	sxtw	x22, w2
	mov	x21, x1
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -48
	.cfi_offset 20, -40
	mov	x19, x0
	smull	x0, w3, w22
	mov	w20, w3
	str	x23, [sp, 48]
	.cfi_offset 23, -16
	lsl	x0, x0, 2
	bl	malloc
	negs	w3, w20
	and	w1, w20, 3
	and	w3, w3, 3
	csneg	w3, w1, w3, mi
	sub	w3, w20, w3
	cmp	w22, 0
	ble	.L23
	sub	w1, w3, #1
	mov	w8, 0
	mov	x7, 0
	lsr	w1, w1, 2
	add	w1, w1, 1
	lsl	w9, w1, 2
	.p2align 3,,7
.L28:
	ldr	s1, [x21, x7, lsl 2]
	dup	v2.4s, v1.s[0]
	cmp	w3, 0
	ble	.L32
	sxtw	x4, w8
	sbfiz	x2, x8, 2, 32
	add	x4, x4, w1, uxtw 2
	lsl	x4, x4, 2
	.p2align 3,,7
.L26:
	ldr	q0, [x19, x2]
	fadd	v0.4s, v0.4s, v2.4s
	str	q0, [x0, x2]
	add	x2, x2, 16
	cmp	x2, x4
	bne	.L26
	mov	w5, w9
.L25:
	cmp	w20, w5
	ble	.L31
	add	w4, w20, w7
	sxtw	x2, w5
	add	x6, x2, w8, sxtw
	sub	w5, w20, w5
	add	x4, x2, w4, sxtw
	mov	x2, 0
	add	x6, x0, x6, lsl 2
	add	x4, x19, x4, lsl 2
	.p2align 3,,7
.L30:
	ldr	s0, [x4, x2, lsl 2]
	fadd	s0, s0, s1
	str	s0, [x6, x2, lsl 2]
	add	x2, x2, 1
	cmp	x5, x2
	bne	.L30
.L31:
	add	x7, x7, 1
	add	w8, w8, w20
	cmp	x22, x7
	bne	.L28
.L23:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 23
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L32:
	.cfi_restore_state
	mov	w5, 0
	b	.L25
	.cfi_endproc
.LFE4375:
	.size	sum_matrix_vector, .-sum_matrix_vector
	.align	2
	.p2align 4,,11
	.global	diff_matrix
	.type	diff_matrix, %function
diff_matrix:
.LFB4376:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	x19, x0
	smull	x0, w3, w2
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -16
	.cfi_offset 22, -8
	mov	w22, w2
	mov	w21, w3
	mov	x20, x1
	lsl	x0, x0, 2
	bl	malloc
	mul	w1, w21, w22
	negs	w2, w1
	and	w3, w1, 3
	and	w2, w2, 3
	csneg	w2, w3, w2, mi
	sub	w2, w1, w2
	cmp	w2, 0
	ble	.L42
	sub	w2, w2, #1
	mov	x3, 0
	lsr	w2, w2, 2
	add	w2, w2, 1
	ubfiz	x4, x2, 4, 31
	.p2align 3,,7
.L39:
	ldr	q0, [x19, x3]
	ldr	q1, [x20, x3]
	fsub	v0.4s, v0.4s, v1.4s
	str	q0, [x0, x3]
	add	x3, x3, 16
	cmp	x4, x3
	bne	.L39
	lsl	w2, w2, 2
.L38:
	cmp	w1, w2
	ble	.L37
	sxtw	x2, w2
	.p2align 3,,7
.L41:
	ldr	s0, [x19, x2, lsl 2]
	ldr	s1, [x20, x2, lsl 2]
	fsub	s0, s0, s1
	str	s0, [x0, x2, lsl 2]
	add	x2, x2, 1
	cmp	w1, w2
	bgt	.L41
.L37:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L42:
	.cfi_restore_state
	mov	w2, 0
	b	.L38
	.cfi_endproc
.LFE4376:
	.size	diff_matrix, .-diff_matrix
	.align	2
	.p2align 4,,11
	.global	relu_matrix
	.type	relu_matrix, %function
relu_matrix:
.LFB4377:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -32
	.cfi_offset 22, -24
	mov	x21, x0
	smull	x0, w1, w2
	mul	w22, w1, w2
	stp	x19, x20, [sp, 16]
	lsl	x0, x0, 2
	.cfi_offset 19, -48
	.cfi_offset 20, -40
	bl	malloc
	negs	w1, w22
	and	w2, w22, 3
	and	w1, w1, 3
	mov	x20, x0
	csneg	w1, w2, w1, mi
	sub	w0, w22, w1
	cmp	w0, 0
	ble	.L53
	sub	w0, w0, #1
	stp	x23, x24, [sp, 48]
	.cfi_offset 24, -8
	.cfi_offset 23, -16
	mov	x19, 0
	lsr	w0, w0, 2
	add	w23, w0, 1
	ubfiz	x24, x23, 4, 31
	.p2align 3,,7
.L48:
	ldr	q0, [x21, x19]
	bl	relu
	str	q0, [x20, x19]
	add	x19, x19, 16
	cmp	x19, x24
	bne	.L48
	lsl	w0, w23, 2
	ldp	x23, x24, [sp, 48]
	.cfi_restore 24
	.cfi_restore 23
.L47:
	movi	v1.2s, #0
	sxtw	x1, w0
	cmp	w22, w0
	ble	.L46
	.p2align 3,,7
.L51:
	ldr	s0, [x21, x1, lsl 2]
	fcmpe	s0, #0.0
	fcsel	s0, s0, s1, gt
	str	s0, [x20, x1, lsl 2]
	add	x1, x1, 1
	cmp	w22, w1
	bgt	.L51
.L46:
	ldp	x21, x22, [sp, 32]
	mov	x0, x20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L53:
	.cfi_restore_state
	mov	w0, 0
	b	.L47
	.cfi_endproc
.LFE4377:
	.size	relu_matrix, .-relu_matrix
	.align	2
	.p2align 4,,11
	.global	relu_derivate_matrix
	.type	relu_derivate_matrix, %function
relu_derivate_matrix:
.LFB4378:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -32
	.cfi_offset 22, -24
	mov	x21, x0
	smull	x0, w1, w2
	mul	w22, w1, w2
	stp	x19, x20, [sp, 16]
	lsl	x0, x0, 2
	.cfi_offset 19, -48
	.cfi_offset 20, -40
	bl	malloc
	negs	w1, w22
	and	w2, w22, 3
	and	w1, w1, 3
	mov	x20, x0
	csneg	w1, w2, w1, mi
	sub	w0, w22, w1
	cmp	w0, 0
	ble	.L65
	sub	w0, w0, #1
	stp	x23, x24, [sp, 48]
	.cfi_offset 24, -8
	.cfi_offset 23, -16
	mov	x19, 0
	lsr	w0, w0, 2
	add	w23, w0, 1
	ubfiz	x24, x23, 4, 31
	.p2align 3,,7
.L60:
	ldr	q0, [x21, x19]
	bl	relu_derivate
	str	q0, [x20, x19]
	add	x19, x19, 16
	cmp	x19, x24
	bne	.L60
	lsl	w0, w23, 2
	ldp	x23, x24, [sp, 48]
	.cfi_restore 24
	.cfi_restore 23
.L59:
	movi	v1.2s, #0
	sxtw	x1, w0
	fmov	s2, 1.0e+0
	cmp	w22, w0
	ble	.L58
	.p2align 3,,7
.L63:
	ldr	s0, [x21, x1, lsl 2]
	fcmpe	s0, #0.0
	fcsel	s0, s2, s1, gt
	str	s0, [x20, x1, lsl 2]
	add	x1, x1, 1
	cmp	w22, w1
	bgt	.L63
.L58:
	ldp	x21, x22, [sp, 32]
	mov	x0, x20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L65:
	.cfi_restore_state
	mov	w0, 0
	b	.L59
	.cfi_endproc
.LFE4378:
	.size	relu_derivate_matrix, .-relu_derivate_matrix
	.align	2
	.p2align 4,,11
	.global	multiply_matrix_scalar
	.type	multiply_matrix_scalar, %function
multiply_matrix_scalar:
.LFB4379:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	str	d8, [sp, 40]
	.cfi_offset 72, -8
	fmov	s8, s0
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	x19, x0
	smull	x0, w2, w1
	mov	w20, w2
	str	x21, [sp, 32]
	.cfi_offset 21, -16
	mov	w21, w1
	lsl	x0, x0, 2
	bl	malloc
	mul	w2, w20, w21
	negs	w1, w2
	and	w3, w2, 3
	and	w1, w1, 3
	csneg	w1, w3, w1, mi
	sub	w1, w2, w1
	cmp	w1, 0
	ble	.L77
	dup	v0.4s, v8.s[0]
	sub	w3, w1, #1
	mov	x1, 0
	lsr	w3, w3, 2
	add	w3, w3, 1
	ubfiz	x4, x3, 4, 31
	.p2align 3,,7
.L73:
	ldr	q1, [x19, x1]
	fmul	v1.4s, v1.4s, v0.4s
	str	q1, [x0, x1]
	add	x1, x1, 16
	cmp	x1, x4
	bne	.L73
	lsl	w3, w3, 2
.L72:
	sxtw	x1, w3
	cmp	w2, w3
	ble	.L71
	.p2align 3,,7
.L75:
	ldr	s1, [x19, x1, lsl 2]
	fmul	s1, s1, s8
	str	s1, [x0, x1, lsl 2]
	add	x1, x1, 1
	cmp	w2, w1
	bgt	.L75
.L71:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldr	d8, [sp, 40]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 72
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L77:
	.cfi_restore_state
	mov	w3, 0
	b	.L72
	.cfi_endproc
.LFE4379:
	.size	multiply_matrix_scalar, .-multiply_matrix_scalar
	.align	2
	.p2align 4,,11
	.global	sum_multiply_matrix_scalar_fast
	.type	sum_multiply_matrix_scalar_fast, %function
sum_multiply_matrix_scalar_fast:
.LFB4380:
	.cfi_startproc
	mul	w3, w3, w2
	negs	w4, w3
	and	w2, w3, 3
	and	w4, w4, 3
	csneg	w4, w2, w4, mi
	sub	w4, w3, w4
	cmp	w4, 0
	ble	.L87
	sub	w4, w4, #1
	mov	x5, 0
	lsr	w4, w4, 2
	add	w4, w4, 1
	ubfiz	x2, x4, 4, 31
	.p2align 3,,7
.L84:
	ldr	q2, [x1, x5]
	ldr	q1, [x0, x5]
	fmul	v2.4s, v2.4s, v0.s[0]
	fadd	v1.4s, v1.4s, v2.4s
	str	q1, [x0, x5]
	add	x5, x5, 16
	cmp	x2, x5
	bne	.L84
	lsl	w4, w4, 2
.L83:
	cmp	w3, w4
	ble	.L82
	sxtw	x4, w4
	.p2align 3,,7
.L86:
	ldr	s2, [x1, x4, lsl 2]
	ldr	s1, [x0, x4, lsl 2]
	fmadd	s1, s2, s0, s1
	str	s1, [x0, x4, lsl 2]
	add	x4, x4, 1
	cmp	w3, w4
	bgt	.L86
.L82:
	ret
	.p2align 2,,3
.L87:
	mov	w4, 0
	b	.L83
	.cfi_endproc
.LFE4380:
	.size	sum_multiply_matrix_scalar_fast, .-sum_multiply_matrix_scalar_fast
	.align	2
	.p2align 4,,11
	.global	max_vector_fast
	.type	max_vector_fast, %function
max_vector_fast:
.LFB4381:
	.cfi_startproc
	negs	w5, w1
	and	w7, w1, 7
	and	w5, w5, 7
	mov	w2, -822149120
	csneg	w5, w7, w5, mi
	asr	w8, w1, 3
	sub	w5, w1, w5
	mov	w9, 8
	fmov	s2, w2
	cbz	w7, .L92
	.p2align 3,,7
.L109:
	add	w8, w8, 1
	mov	w2, 0
	sxtw	x6, w8
	cmp	w5, 0
	ble	.L94
.L93:
	sxtw	x6, w8
	mov	x4, x0
	mov	w3, 0
	.p2align 3,,7
.L96:
	ldp	q0, q1, [x4]
	asr	w2, w3, 1
	add	w3, w3, 8
	add	x4, x4, 32
	add	x2, x6, w2, sxtw
	fmax	v0.4s, v0.4s, v1.4s
	lsl	x2, x2, 2
	str	q0, [x0, x2]
	cmp	w3, w5
	blt	.L96
	cbz	w7, .L104
	sub	w2, w5, #1
	cmp	w5, 0
	and	w2, w2, -8
	add	w2, w2, 8
	csel	w2, w2, w9, gt
.L94:
	cmp	w1, w2
	ble	.L102
	fmov	s0, s2
	sxtw	x2, w2
	.p2align 3,,7
.L99:
	ldr	s1, [x0, x2, lsl 2]
	fcmpe	s1, s0
	bgt	.L103
.L98:
	add	x2, x2, 1
	cmp	w1, w2
	bgt	.L99
	add	x6, x0, x6, lsl 2
	str	s0, [x6, -4]
	cmp	w1, 7
	ble	.L108
.L104:
	mov	w1, w8
.L110:
	negs	w5, w1
	and	w7, w1, 7
	and	w5, w5, 7
	asr	w8, w1, 3
	csneg	w5, w7, w5, mi
	sub	w5, w1, w5
	cbnz	w7, .L109
.L92:
	cmp	w5, 0
	bgt	.L93
	mov	w1, w8
	b	.L110
.L108:
	ret
	.p2align 2,,3
.L103:
	fmov	s0, s1
	b	.L98
.L102:
	fmov	s0, s2
	add	x6, x0, x6, lsl 2
	str	s0, [x6, -4]
	cmp	w1, 7
	bgt	.L104
	b	.L108
	.cfi_endproc
.LFE4381:
	.size	max_vector_fast, .-max_vector_fast
	.align	2
	.p2align 4,,11
	.global	min_vector_fast
	.type	min_vector_fast, %function
min_vector_fast:
.LFB4382:
	.cfi_startproc
	negs	w5, w1
	and	w6, w1, 7
	and	w5, w5, 7
	mov	w2, 1325334528
	csneg	w5, w6, w5, mi
	asr	w7, w1, 3
	sub	w5, w1, w5
	mov	x8, -4
	fmov	s2, w2
	mov	w9, 8
	cbz	w6, .L113
	.p2align 3,,7
.L130:
	add	w7, w7, 1
	mov	w2, 0
	cmp	w5, 0
	ble	.L115
.L114:
	mov	x3, x0
	mov	w2, 0
	.p2align 3,,7
.L117:
	ldp	q0, q1, [x3]
	asr	w4, w2, 1
	add	w2, w2, 8
	add	x3, x3, 32
	sbfiz	x4, x4, 2, 32
	fmin	v0.4s, v0.4s, v1.4s
	str	q0, [x0, x4]
	cmp	w2, w5
	blt	.L117
	cbz	w6, .L125
	sub	w2, w5, #1
	cmp	w5, 0
	and	w2, w2, -8
	add	w2, w2, 8
	csel	w2, w2, w9, gt
.L115:
	cmp	w1, w2
	ble	.L123
	fmov	s0, s2
	sxtw	x2, w2
	.p2align 3,,7
.L120:
	ldr	s1, [x0, x2, lsl 2]
	fcmpe	s1, s0
	bmi	.L124
.L119:
	add	x2, x2, 1
	cmp	w1, w2
	bgt	.L120
	add	x2, x8, w7, sxtw 2
	str	s0, [x0, x2]
	cmp	w1, 7
	ble	.L129
.L125:
	mov	w1, w7
.L131:
	negs	w5, w1
	and	w6, w1, 7
	and	w5, w5, 7
	asr	w7, w1, 3
	csneg	w5, w6, w5, mi
	sub	w5, w1, w5
	cbnz	w6, .L130
.L113:
	cmp	w5, 0
	bgt	.L114
	mov	w1, w7
	b	.L131
.L129:
	ret
	.p2align 2,,3
.L124:
	fmov	s0, s1
	b	.L119
.L123:
	fmov	s0, s2
	add	x2, x8, w7, sxtw 2
	str	s0, [x0, x2]
	cmp	w1, 7
	bgt	.L125
	b	.L129
	.cfi_endproc
.LFE4382:
	.size	min_vector_fast, .-min_vector_fast
	.align	2
	.p2align 4,,11
	.global	copy_vector
	.type	copy_vector, %function
copy_vector:
.LFB4384:
	.cfi_startproc
	cmp	w2, 0
	ble	.L132
	sbfiz	x3, x2, 2, 32
	mov	x2, 0
	.p2align 3,,7
.L134:
	ldr	s0, [x0, x2]
	str	s0, [x1, x2]
	add	x2, x2, 4
	cmp	x3, x2
	bne	.L134
.L132:
	ret
	.cfi_endproc
.LFE4384:
	.size	copy_vector, .-copy_vector
	.align	2
	.p2align 4,,11
	.global	one_hot_matrix
	.type	one_hot_matrix, %function
one_hot_matrix:
.LFB4385:
	.cfi_startproc
	movi	v0.2s, #0
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -16
	.cfi_offset 20, -8
	mov	w19, w2
	mov	x20, x0
	mov	w0, w1
	mov	w1, w2
	bl	init_matrix
	cmp	w19, 0
	ble	.L137
	sxtw	x2, w19
	mov	x1, 0
	fmov	s0, 1.0e+0
	.p2align 3,,7
.L138:
	ldr	w3, [x20, x1, lsl 2]
	madd	w3, w19, w3, w1
	add	x1, x1, 1
	str	s0, [x0, w3, sxtw 2]
	cmp	x2, x1
	bne	.L138
.L137:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE4385:
	.size	one_hot_matrix, .-one_hot_matrix
	.align	2
	.p2align 4,,11
	.global	transpose_matrix
	.type	transpose_matrix, %function
transpose_matrix:
.LFB4386:
	.cfi_startproc
	stp	x29, x30, [sp, -96]!
	.cfi_def_cfa_offset 96
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -80
	.cfi_offset 20, -72
	sxtw	x20, w1
	sbfiz	x19, x20, 2, 32
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	sxtw	x24, w2
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -64
	.cfi_offset 22, -56
	mov	x22, x0
	mov	x23, x20
	mul	x0, x24, x19
	mov	x21, x24
	bl	malloc
	negs	w2, w20
	and	w3, w20, 3
	and	w2, w2, 3
	and	w1, w24, 3
	csneg	w2, w3, w2, mi
	negs	w18, w24
	and	w18, w18, 3
	sub	w2, w20, w2
	csneg	w18, w1, w18, mi
	mov	w1, 0
	sub	w18, w24, w18
	cmp	w2, 0
	ble	.L143
	sub	w1, w18, #1
	lsl	w9, w20, 2
	and	w30, w1, -4
	lsl	w14, w24, 1
	mov	w10, 12
	add	w30, w30, 4
	sbfiz	x9, x9, 2, 32
	add	w16, w14, w24
	mov	w15, w24
	lsl	w13, w24, 2
	mov	x17, x0
	stp	x25, x26, [sp, 64]
	.cfi_offset 26, -24
	.cfi_offset 25, -32
	add	x25, x22, 16
	smull	x10, w20, w10
	lsl	x11, x20, 3
	mov	w12, 0
	and	x20, x1, 4294967292
	mov	x1, 0
	str	x27, [sp, 80]
	.cfi_offset 27, -16
	.p2align 3,,7
.L146:
	mov	w3, 0
	cmp	w18, 0
	ble	.L150
	add	x6, x20, w12, sxtw
	sbfiz	x7, x12, 2, 32
	add	x4, x22, x7
	sbfiz	x26, x15, 2, 32
	add	x6, x25, x6, lsl 2
	sbfiz	x27, x14, 2, 32
	sbfiz	x8, x16, 2, 32
	mov	x3, x17
	.p2align 3,,7
.L144:
	sub	x5, x4, x7
	add	x4, x4, 16
	ldr	q0, [x4, -16]
	ldr	q3, [x5, x26]
	ldr	q2, [x5, x8]
	ldr	q1, [x5, x27]
	zip1	v5.4s, v0.4s, v3.4s
	zip2	v0.4s, v0.4s, v3.4s
	zip1	v4.4s, v1.4s, v2.4s
	zip2	v1.4s, v1.4s, v2.4s
	dup	d3, v5.d[1]
	dup	d2, v0.d[1]
	dup	d6, v4.d[1]
	stp	d5, d4, [x3]
	ins	v0.d[1], v1.d[0]
	dup	d4, v1.d[1]
	mov	v1.8b, v2.8b
	ins	v3.d[1], v6.d[0]
	ins	v1.d[1], v4.d[0]
	str	q3, [x3, x19]
	str	q0, [x3, x11]
	str	q1, [x3, x10]
	add	x3, x3, x9
	cmp	x4, x6
	bne	.L144
	mov	w3, w30
.L150:
	cmp	w21, w3
	bgt	.L145
.L148:
	add	x1, x1, 4
	add	w12, w12, w13
	add	w15, w15, w13
	add	w14, w14, w13
	add	w16, w16, w13
	add	x17, x17, 16
	cmp	w2, w1
	bgt	.L146
	ldp	x25, x26, [sp, 64]
	.cfi_restore 26
	.cfi_restore 25
	sub	w1, w2, #1
	ldr	x27, [sp, 80]
	.cfi_restore 27
	and	w1, w1, -4
	add	w1, w1, 4
.L143:
	cmp	w23, w1
	ble	.L141
	mul	w2, w1, w21
	add	x6, x0, w1, sxtw 2
	.p2align 3,,7
.L152:
	cmp	w21, 0
	ble	.L155
	add	x5, x24, w2, sxtw
	add	x4, x22, w2, sxtw 2
	mov	x3, x6
	add	x5, x22, x5, lsl 2
	.p2align 3,,7
.L153:
	ldr	s0, [x4], 4
	str	s0, [x3]
	add	x3, x3, x19
	cmp	x5, x4
	bne	.L153
.L155:
	add	w1, w1, 1
	add	x6, x6, 4
	add	w2, w2, w21
	cmp	w23, w1
	bne	.L152
.L141:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L145:
	.cfi_def_cfa_offset 96
	.cfi_offset 19, -80
	.cfi_offset 20, -72
	.cfi_offset 21, -64
	.cfi_offset 22, -56
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	.cfi_offset 27, -16
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	sxtw	x5, w3
	mul	w4, w23, w3
	add	x8, x5, w12, sxtw
	add	x7, x5, w15, sxtw
	add	x6, x5, w14, sxtw
	add	x5, x5, w16, sxtw
	lsl	x8, x8, 2
	lsl	x7, x7, 2
	lsl	x6, x6, 2
	lsl	x5, x5, 2
	add	x4, x1, w4, sxtw
	ldr	q3, [x22, x7]
	add	x4, x0, x4, lsl 2
	ldr	q2, [x22, x5]
	ldr	q0, [x22, x8]
	ldr	q4, [x22, x6]
	zip1	v1.4s, v0.4s, v3.4s
	zip2	v0.4s, v0.4s, v3.4s
	zip1	v5.4s, v4.4s, v2.4s
	zip2	v4.4s, v4.4s, v2.4s
	dup	d2, v1.d[1]
	dup	d3, v0.d[1]
	dup	d7, v5.d[1]
	dup	d6, v4.d[1]
	ins	v1.d[1], v5.d[0]
	ins	v0.d[1], v4.d[0]
	ins	v2.d[1], v7.d[0]
	ins	v3.d[1], v6.d[0]
	b	.L147
	.p2align 2,,3
.L156:
	mov	v0.16b, v3.16b
.L147:
	add	w3, w3, 1
	str	q1, [x4]
	add	x4, x4, x19
	mov	v1.16b, v2.16b
	mov	v2.16b, v0.16b
	cmp	w21, w3
	bne	.L156
	b	.L148
	.cfi_endproc
.LFE4386:
	.size	transpose_matrix, .-transpose_matrix
	.align	2
	.p2align 4,,11
	.global	minmax_matrix
	.type	minmax_matrix, %function
minmax_matrix:
.LFB4383:
	.cfi_startproc
	stp	x29, x30, [sp, -144]!
	.cfi_def_cfa_offset 144
	.cfi_offset 29, -144
	.cfi_offset 30, -136
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -128
	.cfi_offset 20, -120
	mov	w20, w1
	sxtw	x1, w2
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -112
	.cfi_offset 22, -104
	sbfiz	x21, x20, 2, 32
	mov	x19, x0
	sxtw	x0, w20
	str	x0, [sp, 128]
	mul	x0, x1, x21
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -96
	.cfi_offset 24, -88
	mov	x24, x1
	stp	x27, x28, [sp, 80]
	.cfi_offset 27, -64
	.cfi_offset 28, -56
	bl	malloc
	mov	x28, x0
	mov	w2, w24
	mov	w1, w20
	mov	x0, x19
	bl	transpose_matrix
	mov	x27, x0
	mov	x0, x21
	bl	malloc
	mov	x21, x0
	negs	w1, w20
	and	w2, w20, 3
	and	w1, w1, 3
	csneg	w1, w2, w1, mi
	sub	w0, w20, w1
	str	w0, [sp, 136]
	cmp	w24, 0
	ble	.L169
	stp	x25, x26, [sp, 64]
	.cfi_offset 26, -72
	.cfi_offset 25, -80
	sub	w25, w0, #1
	sub	w22, w20, #1
	lsr	w25, w25, 2
	add	x22, x22, 1
	add	w0, w25, 1
	mov	x23, 0
	lsl	x1, x22, 2
	mov	w26, 0
	lsl	w25, w0, 2
	stp	d8, d9, [sp, 96]
	.cfi_offset 73, -40
	.cfi_offset 72, -48
	fmov	s8, 1.0e+0
	str	x1, [sp, 120]
	str	w0, [sp, 140]
	.p2align 3,,7
.L168:
	cmp	w20, 0
	ble	.L163
	ldr	x2, [sp, 120]
	lsl	x19, x23, 2
	add	x22, x27, x19
	mov	x0, x21
	mov	x1, x22
	bl	memcpy
	mov	w1, w20
	mov	x0, x21
	bl	min_vector_fast
	fmov	s9, s0
	ldr	x2, [sp, 120]
	mov	x1, x22
	bl	memcpy
	mov	x0, x21
	mov	w1, w20
	bl	max_vector_fast
	fsub	s0, s0, s9
	fneg	s2, s9
	ldr	w0, [sp, 136]
	fdiv	s2, s2, s0
	fdiv	s0, s8, s0
	dup	v3.4s, v2.s[0]
	cmp	w0, 0
	ble	.L177
.L164:
	ldr	w0, [sp, 140]
	mov	x3, x19
	add	x0, x23, w0, uxtw 2
	lsl	x0, x0, 2
	.p2align 3,,7
.L167:
	ldr	q1, [x27, x3]
	fmul	v1.4s, v1.4s, v0.s[0]
	fadd	v1.4s, v3.4s, v1.4s
	str	q1, [x28, x3]
	add	x3, x3, 16
	cmp	x0, x3
	bne	.L167
	mov	w0, w25
	cmp	w20, w25
	ble	.L166
.L165:
	add	x3, x23, w0, sxtw
	sub	w0, w20, w0
	add	x0, x0, x3
	lsl	x3, x3, 2
	lsl	x0, x0, 2
	.p2align 3,,7
.L170:
	ldr	s1, [x27, x3]
	fmadd	s1, s1, s0, s2
	str	s1, [x28, x3]
	add	x3, x3, 4
	cmp	x0, x3
	bne	.L170
.L166:
	ldr	x0, [sp, 128]
	add	w26, w26, 1
	add	x23, x23, x0
	cmp	w24, w26
	bne	.L168
	ldp	x25, x26, [sp, 64]
	.cfi_restore 26
	.cfi_restore 25
	ldp	d8, d9, [sp, 96]
	.cfi_restore 73
	.cfi_restore 72
.L169:
	mov	w2, w20
	mov	w1, w24
	mov	x0, x28
	bl	transpose_matrix
	mov	x19, x0
	mov	x0, x28
	bl	free
	mov	x0, x21
	bl	free
	mov	x0, x27
	bl	free
	ldp	x21, x22, [sp, 32]
	mov	x0, x19
	ldp	x19, x20, [sp, 16]
	ldp	x23, x24, [sp, 48]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 27
	.cfi_restore 28
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L163:
	.cfi_def_cfa_offset 144
	.cfi_offset 19, -128
	.cfi_offset 20, -120
	.cfi_offset 21, -112
	.cfi_offset 22, -104
	.cfi_offset 23, -96
	.cfi_offset 24, -88
	.cfi_offset 25, -80
	.cfi_offset 26, -72
	.cfi_offset 27, -64
	.cfi_offset 28, -56
	.cfi_offset 29, -144
	.cfi_offset 30, -136
	.cfi_offset 72, -48
	.cfi_offset 73, -40
	mov	x0, x21
	mov	w1, w20
	bl	min_vector_fast
	fmov	s3, s0
	mov	w1, w20
	bl	max_vector_fast
	ldr	w0, [sp, 136]
	fsub	s0, s0, s3
	fneg	s2, s3
	fdiv	s2, s2, s0
	fdiv	s0, s8, s0
	dup	v3.4s, v2.s[0]
	cmp	w0, 0
	ble	.L166
	lsl	x19, x23, 2
	b	.L164
	.p2align 2,,3
.L177:
	mov	w0, 0
	b	.L165
	.cfi_endproc
.LFE4383:
	.size	minmax_matrix, .-minmax_matrix
	.align	2
	.p2align 4,,11
	.global	multiply_matrix_matrix
	.type	multiply_matrix_matrix, %function
multiply_matrix_matrix:
.LFB4387:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -16
	.cfi_offset 24, -8
	sxtw	x24, w4
	mov	w23, w2
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -32
	.cfi_offset 22, -24
	mov	x21, x0
	smull	x0, w24, w2
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -48
	.cfi_offset 20, -40
	mov	w19, w3
	mov	x20, x1
	lsl	x0, x0, 2
	bl	malloc
	negs	w10, w19
	and	w1, w19, 3
	and	w10, w10, 3
	csneg	w10, w1, w10, mi
	sub	w10, w19, w10
	cmp	w23, 0
	ble	.L178
	sub	w12, w10, #1
	mov	x22, x24
	mov	w14, 0
	mov	w13, 0
	lsr	w12, w12, 2
	mov	w15, 0
	add	w12, w12, 1
	ubfiz	x6, x12, 4, 31
	lsl	w12, w12, 2
	.p2align 3,,7
.L180:
	cmp	w22, 0
	ble	.L184
	add	x9, x24, w13, sxtw
	sxtw	x11, w14
	add	x8, x0, w13, sxtw 2
	add	x5, x21, w14, sxtw 2
	add	x9, x0, x9, lsl 2
	mov	w7, 0
	.p2align 3,,7
.L187:
	cmp	w10, 0
	ble	.L192
	movi	v0.4s, 0
	add	x2, x20, w7, sxtw 2
	mov	x1, 0
	.p2align 3,,7
.L181:
	ldr	q2, [x5, x1]
	ldr	q1, [x2, x1]
	add	x1, x1, 16
	fmla	v0.4s, v2.4s, v1.4s
	cmp	x1, x6
	bne	.L181
	dup	s4, v0.s[3]
	mov	w3, w12
	dup	s5, v0.s[2]
	dup	s6, v0.s[1]
	fmov	s1, s0
.L186:
	cmp	w19, w3
	ble	.L188
	sxtw	x2, w3
	movi	v0.2s, #0
	add	x4, x11, x2
	add	x2, x2, w7, sxtw
	sub	w3, w19, w3
	mov	x1, 0
	add	x4, x21, x4, lsl 2
	add	x2, x20, x2, lsl 2
	.p2align 3,,7
.L183:
	ldr	s3, [x4, x1, lsl 2]
	ldr	s2, [x2, x1, lsl 2]
	add	x1, x1, 1
	fmadd	s0, s3, s2, s0
	cmp	x1, x3
	bne	.L183
	fadd	s0, s0, s1
	add	w7, w7, w19
	fadd	s0, s0, s6
	fadd	s0, s0, s5
	fadd	s0, s0, s4
	str	s0, [x8], 4
	cmp	x9, x8
	bne	.L187
.L184:
	add	w15, w15, 1
	add	w13, w13, w22
	add	w14, w14, w19
	cmp	w23, w15
	bne	.L180
.L178:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L188:
	.cfi_restore_state
	movi	v0.2s, #0
	add	w7, w7, w19
	fadd	s0, s0, s1
	fadd	s0, s0, s6
	fadd	s0, s0, s5
	fadd	s0, s0, s4
	str	s0, [x8], 4
	cmp	x9, x8
	bne	.L187
	b	.L184
	.p2align 2,,3
.L192:
	movi	v1.2s, #0
	mov	w3, 0
	fmov	s6, s1
	fmov	s5, s1
	fmov	s4, s1
	b	.L186
	.cfi_endproc
.LFE4387:
	.size	multiply_matrix_matrix, .-multiply_matrix_matrix
	.align	2
	.p2align 4,,11
	.global	multiply_matrix_hadamard
	.type	multiply_matrix_hadamard, %function
multiply_matrix_hadamard:
.LFB4388:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	x19, x0
	smull	x0, w3, w2
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -16
	.cfi_offset 22, -8
	mov	w22, w2
	mov	w21, w3
	mov	x20, x1
	lsl	x0, x0, 2
	bl	malloc
	mul	w1, w21, w22
	negs	w2, w1
	and	w3, w1, 3
	and	w2, w2, 3
	csneg	w2, w3, w2, mi
	sub	w2, w1, w2
	cmp	w2, 0
	ble	.L198
	sub	w2, w2, #1
	mov	x3, 0
	lsr	w2, w2, 2
	add	w2, w2, 1
	ubfiz	x4, x2, 4, 31
	.p2align 3,,7
.L195:
	ldr	q0, [x19, x3]
	ldr	q1, [x20, x3]
	fmul	v0.4s, v0.4s, v1.4s
	str	q0, [x0, x3]
	add	x3, x3, 16
	cmp	x4, x3
	bne	.L195
	lsl	w2, w2, 2
.L194:
	cmp	w1, w2
	ble	.L193
	sxtw	x2, w2
	.p2align 3,,7
.L197:
	ldr	s0, [x19, x2, lsl 2]
	ldr	s1, [x20, x2, lsl 2]
	fmul	s0, s0, s1
	str	s0, [x0, x2, lsl 2]
	add	x2, x2, 1
	cmp	w1, w2
	bgt	.L197
.L193:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L198:
	.cfi_restore_state
	mov	w2, 0
	b	.L194
	.cfi_endproc
.LFE4388:
	.size	multiply_matrix_hadamard, .-multiply_matrix_hadamard
	.align	2
	.p2align 4,,11
	.global	compare_vector
	.type	compare_vector, %function
compare_vector:
.LFB4389:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	w19, w2
	mov	x20, x0
	sbfiz	x0, x2, 2, 32
	str	x21, [sp, 32]
	.cfi_offset 21, -16
	mov	x21, x1
	bl	malloc
	negs	w3, w19
	and	w1, w19, 3
	and	w3, w3, 3
	csneg	w3, w1, w3, mi
	sub	w3, w19, w3
	cmp	w3, 0
	ble	.L207
	mov	x1, 0
	sub	w3, w3, #1
	movi	v0.4s, 0x1
	mov	x4, x0
	ldr	q1, [x21, x1]
	lsr	w3, w3, 2
	ldr	q2, [x20, x1]
	add	w3, w3, 1
	add	x1, x0, w3, uxtw 4
	cmeq	v1.4s, v1.4s, v2.4s
	and	v0.16b, v0.16b, v1.16b
	.p2align 3,,7
.L204:
	str	q0, [x4], 16
	cmp	x4, x1
	bne	.L204
	lsl	w3, w3, 2
.L203:
	cmp	w19, w3
	ble	.L202
	sxtw	x3, w3
	.p2align 3,,7
.L206:
	ldr	w2, [x21, x3, lsl 2]
	ldr	w4, [x20, x3, lsl 2]
	cmp	w4, w2
	cset	w2, eq
	str	w2, [x0, x3, lsl 2]
	add	x3, x3, 1
	cmp	w19, w3
	bgt	.L206
.L202:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L207:
	.cfi_restore_state
	mov	w3, 0
	b	.L203
	.cfi_endproc
.LFE4389:
	.size	compare_vector, .-compare_vector
	.align	2
	.p2align 4,,11
	.global	matrix_redux_float
	.type	matrix_redux_float, %function
matrix_redux_float:
.LFB4390:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -16
	.cfi_offset 22, -8
	sbfiz	x21, x1, 2, 32
	mov	w22, w1
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	w19, w2
	mov	x20, x0
	mov	x0, x21
	bl	malloc
	negs	w6, w19
	and	w1, w19, 3
	and	w6, w6, 3
	csneg	w6, w1, w6, mi
	sub	w6, w19, w6
	cmp	w22, 0
	ble	.L211
	sub	w7, w6, #1
	add	x8, x21, x0
	and	w9, w7, -4
	mov	x2, x0
	and	x7, x7, 4294967292
	add	w9, w9, 4
	add	x10, x20, 16
	mov	w5, 0
	.p2align 3,,7
.L213:
	cmp	w6, 0
	ble	.L223
	add	x3, x7, w5, sxtw
	add	x1, x20, w5, sxtw 2
	movi	v0.4s, 0
	add	x3, x10, x3, lsl 2
	.p2align 3,,7
.L214:
	ldr	q1, [x1], 16
	fadd	v0.4s, v0.4s, v1.4s
	cmp	x3, x1
	bne	.L214
	dup	s3, v0.s[3]
	mov	w4, w9
	dup	s4, v0.s[2]
	dup	s5, v0.s[1]
	fmov	s1, s0
.L218:
	cmp	w19, w4
	ble	.L219
	sxtw	x3, w5
	movi	v0.2s, #0
	add	x3, x3, w4, sxtw
	sub	w4, w19, w4
	mov	x1, 0
	add	x3, x20, x3, lsl 2
	.p2align 3,,7
.L216:
	ldr	s2, [x3, x1, lsl 2]
	add	x1, x1, 1
	fadd	s0, s0, s2
	cmp	x1, x4
	bne	.L216
	fadd	s0, s0, s1
	add	w5, w5, w19
	fadd	s0, s0, s5
	fadd	s0, s0, s4
	fadd	s0, s0, s3
	str	s0, [x2], 4
	cmp	x2, x8
	bne	.L213
.L211:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L219:
	.cfi_restore_state
	movi	v0.2s, #0
	add	w5, w5, w19
	fadd	s0, s0, s1
	fadd	s0, s0, s5
	fadd	s0, s0, s4
	fadd	s0, s0, s3
	str	s0, [x2], 4
	cmp	x2, x8
	bne	.L213
	b	.L211
	.p2align 2,,3
.L223:
	movi	v1.2s, #0
	mov	w4, 0
	fmov	s5, s1
	fmov	s4, s1
	fmov	s3, s1
	b	.L218
	.cfi_endproc
.LFE4390:
	.size	matrix_redux_float, .-matrix_redux_float
	.align	2
	.p2align 4,,11
	.global	matrix_redux_int
	.type	matrix_redux_int, %function
matrix_redux_int:
.LFB4391:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -16
	.cfi_offset 22, -8
	sbfiz	x22, x1, 2, 32
	mov	w21, w1
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	mov	w19, w2
	mov	x20, x0
	mov	x0, x22
	bl	malloc
	negs	w12, w19
	and	w1, w19, 3
	and	w12, w12, 3
	csneg	w12, w1, w12, mi
	sub	w12, w19, w12
	cmp	w21, 0
	ble	.L224
	sub	w13, w12, #1
	add	x14, x22, x0
	and	w15, w13, -4
	mov	x2, x0
	and	x13, x13, 4294967292
	add	w15, w15, 4
	add	x16, x20, 16
	mov	w9, 0
	.p2align 3,,7
.L226:
	cmp	w12, 0
	ble	.L235
	add	x3, x13, w9, sxtw
	add	x1, x20, w9, sxtw 2
	movi	v0.4s, 0
	add	x3, x16, x3, lsl 2
	.p2align 3,,7
.L227:
	ldr	q1, [x1], 16
	add	v0.4s, v0.4s, v1.4s
	cmp	x3, x1
	bne	.L227
	umov	w10, v0.s[3]
	umov	w11, v0.s[2]
	umov	w8, v0.s[1]
	fmov	w3, s0
	mov	w6, w15
.L231:
	cmp	w19, w6
	ble	.L228
	sxtw	x1, w9
	sub	w7, w19, w6
	add	x6, x1, w6, sxtw
	mov	w4, 0
	mov	x1, 0
	add	x6, x20, x6, lsl 2
	.p2align 3,,7
.L229:
	ldr	w5, [x6, x1, lsl 2]
	add	x1, x1, 1
	add	w4, w4, w5
	cmp	x1, x7
	bne	.L229
	add	w3, w3, w4
.L228:
	add	w1, w8, w3
	add	w9, w9, w19
	add	w1, w1, w11
	add	w1, w1, w10
	str	w1, [x2], 4
	cmp	x2, x14
	bne	.L226
.L224:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L235:
	.cfi_restore_state
	mov	w3, 0
	mov	w8, 0
	mov	w11, 0
	mov	w10, 0
	mov	w6, 0
	b	.L231
	.cfi_endproc
.LFE4391:
	.size	matrix_redux_int, .-matrix_redux_int
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"\t%f "
	.text
	.align	2
	.p2align 4,,11
	.global	print_matrix
	.type	print_matrix, %function
print_matrix:
.LFB4392:
	.cfi_startproc
	stp	x29, x30, [sp, -96]!
	.cfi_def_cfa_offset 96
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	mov	x29, sp
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	mov	w24, w2
	stp	x25, x26, [sp, 64]
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	mov	x25, x0
	mov	w26, w1
	mov	w0, 10
	bl	putchar
	cmp	w26, 0
	ble	.L237
	stp	x21, x22, [sp, 32]
	.cfi_offset 22, -56
	.cfi_offset 21, -64
	adrp	x21, .LC0
	add	x21, x21, :lo12:.LC0
	str	x27, [sp, 80]
	.cfi_offset 27, -16
	sxtw	x27, w24
	mov	w22, 0
	mov	w23, 0
	stp	x19, x20, [sp, 16]
	.cfi_offset 20, -72
	.cfi_offset 19, -80
	.p2align 3,,7
.L238:
	cmp	w24, 0
	ble	.L241
	add	x20, x27, w22, sxtw
	add	x19, x25, w22, sxtw 2
	add	x20, x25, x20, lsl 2
	.p2align 3,,7
.L239:
	ldr	s0, [x19], 4
	mov	x0, x21
	fcvt	d0, s0
	bl	printf
	cmp	x19, x20
	bne	.L239
.L241:
	mov	w0, 10
	add	w23, w23, 1
	bl	putchar
	add	w22, w22, w24
	cmp	w26, w23
	bne	.L238
	ldp	x19, x20, [sp, 16]
	.cfi_restore 20
	.cfi_restore 19
	ldp	x21, x22, [sp, 32]
	.cfi_restore 22
	.cfi_restore 21
	ldr	x27, [sp, 80]
	.cfi_restore 27
.L237:
	mov	w0, 10
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_def_cfa_offset 0
	b	putchar
	.cfi_endproc
.LFE4392:
	.size	print_matrix, .-print_matrix
	.section	.rodata.str1.8
	.align	3
.LC1:
	.string	"\t%d "
	.text
	.align	2
	.p2align 4,,11
	.global	print_int_matrix
	.type	print_int_matrix, %function
print_int_matrix:
.LFB4393:
	.cfi_startproc
	stp	x29, x30, [sp, -96]!
	.cfi_def_cfa_offset 96
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	mov	x29, sp
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	mov	w24, w2
	stp	x25, x26, [sp, 64]
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	mov	x25, x0
	mov	w26, w1
	mov	w0, 10
	bl	putchar
	cmp	w26, 0
	ble	.L246
	stp	x21, x22, [sp, 32]
	.cfi_offset 22, -56
	.cfi_offset 21, -64
	adrp	x21, .LC1
	add	x21, x21, :lo12:.LC1
	str	x27, [sp, 80]
	.cfi_offset 27, -16
	sxtw	x27, w24
	mov	w22, 0
	mov	w23, 0
	stp	x19, x20, [sp, 16]
	.cfi_offset 20, -72
	.cfi_offset 19, -80
	.p2align 3,,7
.L247:
	cmp	w24, 0
	ble	.L250
	add	x20, x27, w22, sxtw
	add	x19, x25, w22, sxtw 2
	add	x20, x25, x20, lsl 2
	.p2align 3,,7
.L248:
	ldr	w1, [x19], 4
	mov	x0, x21
	bl	printf
	cmp	x19, x20
	bne	.L248
.L250:
	mov	w0, 10
	add	w23, w23, 1
	bl	putchar
	add	w22, w22, w24
	cmp	w26, w23
	bne	.L247
	ldp	x19, x20, [sp, 16]
	.cfi_restore 20
	.cfi_restore 19
	ldp	x21, x22, [sp, 32]
	.cfi_restore 22
	.cfi_restore 21
	ldr	x27, [sp, 80]
	.cfi_restore 27
.L246:
	mov	w0, 10
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_def_cfa_offset 0
	b	putchar
	.cfi_endproc
.LFE4393:
	.size	print_int_matrix, .-print_int_matrix
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
