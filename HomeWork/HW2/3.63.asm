<switch_prob>
90 sub $0x3c, %rsi//n-=60
94 cmp $0x5, %rsi
98 ja 4005c3//if (n-60)>5 goto c3 [r=x-0x4b]
9a jmpq 0x4006f8(,%rsi, 8)//else [n<=65] goto [6f8+8*%rsi]'jmptb[n-60]'
a1 lea 0x0(,%rdi,8),%rax //rax=8*rdi [r=8x]
a8 nop
a9 retq
aa mov %rdi, %rax;r=x
ad sar $0x3, %rax;r>>=3
b1 retq
b2 mov %rdi, %rax
b5 shl $0x4, %rax
b9 sub %rdi, %rax
bc mov %rax, %rdi
bf imul %rdi,%rdi
c3 lea 0x4b(%rdi), %rax
c7 retq


6f8: a1 c3
708: a1 aa
718: b2 bf