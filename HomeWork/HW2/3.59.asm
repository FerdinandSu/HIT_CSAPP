store_prod://%rdi: *dest, %rsi x, %rdx y
    movq %rdx, %rax//将y复制到%rax
    cqto//符号扩展%rax的64位值[y]为%rdx:%rax的128位值Y:y
    movq %rsi, %rcx//把x复制到%rcx
    sarq $63, %rcx//x算术右移63位，这样%rcx:%rsi就是全128位值X:x
    //*dest=[x]*[y]=(Xw+x)(Yw+y)=XYww+(Xy+xY)w+xy=(Xy+xY)w+xy
    imulq %rax, %rcx//计算Xy，保存至%rcx
    imulq %rsi, %rdx//计算xY，保存至%rdx
    addq %rdx, %rcx//计算Xy+xY，保存至%rcx
    mulq %rsi//无符号计算xy(结果为128位)，保存至%rdx:%rax
    addq %rcx, %rdx//将xy和w(Xy+xY)的高64位相加
    movq %rax, (%rdi)//把低64位保存至[%rdi]
    movq %rdx, 8(%rdi)//高64位保存至[%rdi+8]
    ret//返回