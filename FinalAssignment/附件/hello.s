	.text
	.file	"hello.c"
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpl	$3, -8(%rbp)
	je	.LBB0_2
# %bb.1:
	movabsq	$.L.str, %rdi
	movb	$0, %al
	callq	printf
	movl	$1, %edi
	movl	%eax, -24(%rbp)         # 4-byte Spill
	callq	exit
.LBB0_2:
	movl	$0, -20(%rbp)
.LBB0_3:                                # =>This Inner Loop Header: Depth=1
	cmpl	$10, -20(%rbp)
	jge	.LBB0_6
# %bb.4:                                #   in Loop: Header=BB0_3 Depth=1
	movabsq	$.L.str.1, %rdi
	movq	-16(%rbp), %rax
	movq	8(%rax), %rsi
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdx
	movb	$0, %al
	callq	printf
	movl	sleepsecs, %edi
	movl	%eax, -28(%rbp)         # 4-byte Spill
	callq	sleep
	movl	%eax, -32(%rbp)         # 4-byte Spill
# %bb.5:                                #   in Loop: Header=BB0_3 Depth=1
	movl	-20(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -20(%rbp)
	jmp	.LBB0_3
.LBB0_6:
	callq	getchar
	xorl	%ecx, %ecx
	movl	%eax, -36(%rbp)         # 4-byte Spill
	movl	%ecx, %eax
	addq	$48, %rsp
	popq	%rbp
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	sleepsecs,@object       # @sleepsecs
	.data
	.globl	sleepsecs
	.p2align	2
sleepsecs:
	.long	2                       # 0x2
	.size	sleepsecs, 4

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Usage: Hello \345\255\246\345\217\267 \345\247\223\345\220\215\357\274\201\n"
	.size	.L.str, 31

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"Hello %s %s\n"
	.size	.L.str.1, 13


	.ident	"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"
	.section	".note.GNU-stack","",@progbits
