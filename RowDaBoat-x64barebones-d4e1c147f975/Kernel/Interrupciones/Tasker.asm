GLOBAL run

section .text

run:
    push rbp
    mov rbp,rsp
    mov rsp,rsi
    mov rax,[rbp+8]
    push rax
    jmp rdi

