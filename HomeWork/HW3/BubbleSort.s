	.file	"BubbleSort.c"
	.text
	.globl	bubble_a
	.type	bubble_a, @function
bubble_a:
.LFB0:
	.cfi_startproc
	subq	$1, %rsi
	jmp	.L2
.L3:
	addl	$1, %eax
.L5:
	movslq	%eax, %rdx
	cmpq	%rsi, %rdx
	jge	.L7
	movslq	%eax, %rdx
	leaq	8(%rdi,%rdx,8), %r8
	movq	(%r8), %rcx
	leaq	(%rdi,%rdx,8), %rdx
	movq	(%rdx), %r9
	cmpq	%r9, %rcx
	jge	.L3
	movq	%r9, (%r8)
	movq	%rcx, (%rdx)
	jmp	.L3
.L7:
	subq	$1, %rsi
.L2:
	testq	%rsi, %rsi
	jle	.L8
	movl	$0, %eax
	jmp	.L5
.L8:
	rep ret
	.cfi_endproc
.LFE0:
	.size	bubble_a, .-bubble_a
	.globl	bubble_ptr
	.type	bubble_ptr, @function
bubble_ptr:
.LFB1:
	.cfi_startproc
	leaq	-8(%rdi,%rsi,8), %rsi
	jmp	.L10
.L11:
	addq	$8, %rax
.L13:
	cmpq	%rax, %rsi
	jbe	.L15
	movq	8(%rax), %rdx
	movq	(%rax), %rcx
	cmpq	%rcx, %rdx
	jge	.L11
	movq	%rcx, 8(%rax)
	movq	%rdx, (%rax)
	jmp	.L11
.L15:
	subq	$8, %rsi
.L10:
	cmpq	%rdi, %rsi
	jbe	.L16
	movq	%rdi, %rax
	jmp	.L13
.L16:
	rep ret
	.cfi_endproc
.LFE1:
	.size	bubble_ptr, .-bubble_ptr
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
