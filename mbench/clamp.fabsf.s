	.file	"clamp.fabsf.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC5:
	.string	"%f secs\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB571:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movl	$400000000, %edi
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	xorl	%ebp, %ebp
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
.L3:
	call	rand
	vcvtsi2ss	%eax, %xmm1, %xmm1
	vmulss	.LC0(%rip), %xmm1, %xmm1
	vaddss	%xmm1, %xmm1, %xmm1
	vsubss	.LC1(%rip), %xmm1, %xmm1
	vmovss	%xmm1, 12(%rsp)
	call	SDL_GetPerformanceCounter
	vmovss	12(%rsp), %xmm1
	movq	%rax, %r12
	vaddss	.LC2(%rip), %xmm1, %xmm2
	vsubss	.LC2(%rip), %xmm1, %xmm0
	vandps	.LC3(%rip), %xmm0, %xmm0
	vsubss	%xmm0, %xmm2, %xmm0
	vmulss	.LC4(%rip), %xmm0, %xmm0
	vmovaps	%xmm0, %xmm1
	vandps	.LC3(%rip), %xmm1, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmulss	.LC4(%rip), %xmm0, %xmm0
	vmovss	%xmm0, 0(%r13,%rbx)
	addq	$4, %rbx
	call	SDL_GetPerformanceCounter
	subq	%r12, %rax
	addq	%rax, %rbp
	cmpq	$400000000, %rbx
	jne	.L3
	movq	%r13, %rdi
	call	free
	call	SDL_GetPerformanceFrequency
	testq	%rbp, %rbp
	js	.L4
	vcvtsi2sdq	%rbp, %xmm0, %xmm0
.L5:
	testq	%rax, %rax
	js	.L6
	vcvtsi2sdq	%rax, %xmm1, %xmm1
.L7:
	vdivsd	%xmm1, %xmm0, %xmm0
	movl	$.LC5, %esi
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
.L4:
	.cfi_restore_state
	movq	%rbp, %rdx
	andl	$1, %ebp
	shrq	%rdx
	orq	%rbp, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	jmp	.L5
.L6:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm1, %xmm1
	vaddsd	%xmm1, %xmm1, %xmm1
	jmp	.L7
	.cfi_endproc
.LFE571:
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
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	805306368
	.align 4
.LC1:
	.long	1065353216
	.align 4
.LC2:
	.long	1062204133
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC3:
	.long	2147483647
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst4
	.align 4
.LC4:
	.long	1056964608
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
