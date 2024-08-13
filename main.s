	.arch armv8-a
	.file	"main.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"\033[36mMatrizes operando:"
	.align	3
.LC1:
	.string	"\033[0m"
	.align	3
.LC2:
	.string	"\033[34mSum A fvec"
	.align	3
.LC3:
	.string	"\033[35mHadamard A B"
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 4,,11
	.global	main
	.type	main, %function
main:
.LFB4372:
	.cfi_startproc
	stp	x29, x30, [sp, -80]!
	.cfi_def_cfa_offset 80
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	mov	x0, 140
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	.cfi_offset 21, -48
	.cfi_offset 22, -40
	.cfi_offset 23, -32
	.cfi_offset 24, -24
	.cfi_offset 25, -16
	.cfi_offset 26, -8
	bl	malloc
	mov	x20, x0
	mov	x0, 140
	bl	malloc
	mov	x22, x0
	mov	x0, 140
	bl	malloc
	mov	x21, x0
	mov	x0, 252
	bl	malloc
	mov	x19, x0
	mov	x0, 20
	bl	malloc
	mov	x23, x0
	mov	x0, 20
	bl	malloc
	mov	x26, x0
	mov	x0, 20
	bl	malloc
	mov	x25, x0
	mov	x0, 140
	bl	malloc
	mov	w9, 61681
	mov	w12, 52429
	mov	x8, x20
	mov	x24, x0
	mov	x11, 0
	mov	w7, 13
	mov	w0, 10
	mov	x10, 0
	fmov	d2, 1.0e+0
	fmov	d4, 4.0e+0
	fmov	d3, -4.0e+0
	movk	w9, 0xf0f0, lsl 16
	fmov	d6, 5.0e+0
	fmov	d5, 7.0e+0
	mov	w13, 5
	movk	w12, 0xcccc, lsl 16
	.p2align 3,,7
.L2:
	mov	w6, w10
	mov	w15, w10
	mov	x3, x11
	mov	w5, w7
	add	w14, w0, 1
	mov	x2, 0
	b	.L5
	.p2align 2,,3
.L15:
	fdiv	d0, d3, d0
	fcvt	s0, d0
.L4:
	umull	x1, w5, w9
	add	w4, w14, w4
	str	s0, [x8, x2, lsl 2]
	add	x2, x2, 1
	scvtf	s1, w4
	ldr	s0, [x20, x3]
	lsr	x1, x1, 36
	add	w1, w1, w1, lsl 4
	str	s0, [x22, x3]
	sub	w1, w5, w1
	str	s1, [x21, x3]
	str	w1, [x24, x3]
	add	w5, w5, w7
	add	x3, x3, 4
	cmp	x2, 7
	beq	.L14
.L5:
	add	w1, w6, w2
	mov	w4, w2
	scvtf	d0, w1
	fadd	d0, d0, d2
	tbz	x2, 0, .L15
	fdiv	d0, d4, d0
	fcvt	s0, d0
	b	.L4
	.p2align 2,,3
.L14:
	scvtf	d0, w15
	sub	w6, w13, w6
	add	w7, w7, 13
	add	x8, x8, 28
	add	w0, w0, 10
	add	x11, x11, 28
	mul	w3, w6, w6
	fmul	d0, d0, d6
	mul	w6, w3, w6
	fdiv	d0, d0, d5
	umull	x2, w3, w12
	umull	x1, w6, w12
	lsr	x2, x2, 35
	add	w2, w2, w2, lsl 2
	lsr	x1, x1, 35
	sub	w2, w3, w2, lsl 1
	str	w2, [x26, x10, lsl 2]
	add	w1, w1, w1, lsl 2
	sub	w1, w6, w1, lsl 1
	str	w1, [x25, x10, lsl 2]
	fcvt	s0, d0
	str	s0, [x23, x10, lsl 2]
	add	x10, x10, 1
	cmp	w7, 78
	bne	.L2
	mov	x5, x19
	mov	w4, 0
	mov	w3, 17
	.p2align 3,,7
.L6:
	mov	x19, 0
	.p2align 3,,7
.L7:
	add	w2, w4, w19
	udiv	w1, w3, w2
	msub	w1, w1, w2, w3
	scvtf	s0, w1
	str	s0, [x5, x19, lsl 2]
	add	x19, x19, 1
	cmp	x19, 7
	bne	.L7
	add	w4, w4, 1
	add	x5, x5, 28
	cmp	w4, 9
	bne	.L6
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	puts
	mov	w2, w19
	mov	x0, x20
	mov	w1, 5
	bl	print_matrix
	mov	w2, w19
	mov	x0, x21
	mov	w1, 5
	bl	print_matrix
	mov	x0, x23
	mov	w2, 1
	mov	w1, 5
	bl	print_matrix
	mov	w2, w19
	mov	w1, 5
	mov	x0, x24
	adrp	x24, .LC1
	bl	print_int_matrix
	add	x24, x24, :lo12:.LC1
	mov	x0, x24
	bl	puts
	mov	w3, w19
	mov	x1, x23
	mov	x0, x20
	mov	w2, 5
	bl	sum_matrix_vector
	mov	x25, x0
	mov	w3, w19
	mov	x1, x21
	mov	w2, 5
	mov	x0, x20
	bl	multiply_matrix_hadamard
	mov	x23, x0
	adrp	x1, .LC2
	add	x0, x1, :lo12:.LC2
	bl	puts
	mov	w2, w19
	mov	w1, 5
	mov	x0, x25
	bl	print_matrix
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	puts
	mov	w2, w19
	mov	w1, 5
	mov	x0, x23
	bl	print_matrix
	mov	x0, x24
	bl	printf
	mov	x0, x20
	bl	free
	mov	x0, x21
	bl	free
	mov	x0, x22
	bl	free
	ldp	x19, x20, [sp, 16]
	mov	w0, 0
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE4372:
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
