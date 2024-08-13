	.arch armv8-a
	.file	"vbslq_example.c"
	.text
	.align	2
	.p2align 4,,11
	.global	vbslq_test
	.type	vbslq_test, %function
vbslq_test:
.LFB4350:
	.cfi_startproc
	bsl	v0.16b, v1.16b, v2.16b
	fadd	v0.4s, v2.4s, v0.4s
	ret
	.cfi_endproc
.LFE4350:
	.size	vbslq_test, .-vbslq_test
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
