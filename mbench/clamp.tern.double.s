	.file	"clamp.tern.double.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	"%f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB589:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movl	$160000000, %edi
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	xorl	%ebx, %ebx
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	call	malloc
	movl	$12412, %edi
	movq	%rax, %r13
	call	srand
	.p2align 4,,10
	.p2align 3
.L5:
	call	rand
	vcvtsi2sd	%eax, %xmm0, %xmm0
	vdivsd	.LC2(%rip), %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vsubsd	.LC3(%rip), %xmm0, %xmm0
	vmovsd	%xmm0, 8(%rsp)
	call	SDL_GetPerformanceCounter
	vmovsd	.LC0(%rip), %xmm1
	vmovsd	8(%rsp), %xmm0
	movq	%rax, %r12
	vucomisd	%xmm1, %xmm0
	ja	.L2
	vxorpd	%xmm2, %xmm2, %xmm2
	vmaxsd	%xmm0, %xmm2, %xmm1
.L2:
	vmovsd	%xmm1, 0(%r13,%rbx)
	addq	$8, %rbx
	call	SDL_GetPerformanceCounter
	subq	%r12, %rax
	addq	%rax, %rbp
	cmpq	$160000000, %rbx
	jne	.L5
	movq	%r13, %rdi
	call	free
	call	SDL_GetPerformanceFrequency
	testq	%rbp, %rbp
	js	.L6
	vcvtsi2sdq	%rbp, %xmm0, %xmm0
.L7:
	testq	%rax, %rax
	js	.L8
	vcvtsi2sdq	%rax, %xmm1, %xmm1
.L9:
	vdivsd	%xmm1, %xmm0, %xmm0
	movl	$.LC4, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	movq	%rbp, %rdx
	andl	$1, %ebp
	shrq	%rdx
	orq	%rbp, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	jmp	.L7
.L8:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm1, %xmm1
	vaddsd	%xmm1, %xmm1, %xmm1
	jmp	.L9
	.cfi_endproc
.LFE589:
	.size	main, .-main
	.globl	D_MAX
	.section	.rodata
	.align 8
	.type	D_MAX, @object
	.size	D_MAX, 8
D_MAX:
	.long	2446413372
	.long	1072299612
	.globl	F_MAX
	.align 4
	.type	F_MAX, @object
	.size	F_MAX, 4
F_MAX:
	.long	1062204133
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	2446413372
	.long	1072299612
	.align 8
.LC2:
	.long	4290772992
	.long	1105199103
	.align 8
.LC3:
	.long	0
	.long	1072693248
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
