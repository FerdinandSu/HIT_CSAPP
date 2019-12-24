process:
    mov rax, rdi
    mov rdx, [rsp+24]
    mov rdx, [rdx]
    mov rcx, [rsp+16]
    mov [rdi], rcx
    mov rcx, [rsp+8]
    mov [rdi+8], rcx
    mov [rdi+16], rdx
    ret

eval:
    sub rsp, 104
    mov [rsp+24], rdx
    lea rax, [rsp+24]
    mov [rsp], rdi
    mov [rsp+8], rsi
    mov [rsp+16], rax
    lea rdi, [rsp+64]
    call process
    mov rax, [rsp+72]
    add rax, [rsp+64]
    add rax, [rsp+80]
    add rsp, 104
    ret