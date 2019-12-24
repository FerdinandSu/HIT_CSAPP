bubble_ptr:
	pushq %rbp
    ;%rdi:data
    ;%rsi:count
	irmovq $1,%rax
    subq %rax,%rsi;count-1
	addq %rsi,%rsi;2(count-1)
	addq %rsi,%rsi;4(count-1)
	addq %rsi,%rsi;8(count-1)
	rrmovq %rdi %rax
	addq %rax,%rsi;%rsi=last
	irmovq $8,%rcx
	jmp	.L10
.L11:
	addq	%rcx, %rax
.L13:
	rrmovq 	%rsi,%rdx
	subq	%rax, %rdx
	jle	.L15
	mrmovq	8(%rax), %rdx;%rdx=first[1]
	mrmovq	(%rax), %r8;%r8=*first
	rrmovq 	%rdx,%r9
	subq	%r8, %r9
	jge	.L11
	rmmovq	%r8, 8(%rax)
	rmmovq	%rdx, (%rax)
	jmp	.L11
.L15:
	subq	%rcx, %rsi;last--
.L10:
	rrmovq 	%rsi,%rdx
	subq	%rdi, %rdx
	jle	.L16
	rrmovq	%rdi, %rax;%rax=first
	jmp	.L13
.L16:
	ret