	.file	"clamp.tern.float.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
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
.L5:
	call	rand
	vcvtsi2ss	%eax, %xmm0, %xmm0
	vmulss	.LC2(%rip), %xmm0, %xmm0
	vaddss	%xmm0, %xmm0, %xmm0
	vsubss	.LC3(%rip), %xmm0, %xmm0
	vmovss	%xmm0, 12(%rsp)
	call	SDL_GetPerformanceCounter
	vmovss	.LC0(%rip), %xmm1
	vmovss	12(%rsp), %xmm0
	movq	%rax, %r12
	vucomiss	%xmm1, %xmm0
	ja	.L2
	vxorps	%xmm2, %xmm2, %xmm2
	vmaxss	%xmm0, %xmm2, %xmm1
.L2:
	vmovss	%xmm1, 0(%r13,%rbx)
	addq	$4, %rbx
	call	SDL_GetPerformanceCounter
	subq	%r12, %rax
	addq	%rax, %rbp
	cmpq	$400000000, %rbx
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
	.long	1062204133
	.align 4
.LC2:
	.long	805306368
	.align 4
.LC3:
	.long	1065353216
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
