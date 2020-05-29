GLOBAL registers

SECTION .text

%macro regDispatcher 2
    push rdi
    push rsi
	mov rdi, %1 ;pasaje de parametro 1 -> # reg
	mov rsi, %2 ;pasaje de parametro 2 -> value of # reg
    pop rsi
    pop rdi
%endmacro

;con todo el llamado entre funciones, recien tengo el PUSHSTATE
;en [rbp+8*4*2]
registers:
    push rbp ;rbp4
    mov rbp,rsp
    push rcx
    push rax
    push rbx
    lea rcx,[rbp + 8*8] ;rcx -> r15
    mov rax,15
    mov rbx,8
    mul rbx
    add rcx,rax ;rcx -> rip
    mov rax,16 ;acumulador
.loop:
    cmp rax,0
    je .end ;if no regs are left, finish
    regDispatcher rax, [rcx];then continue 
    sub rcx,8 ;next reg from bottom to top
    dec rax
    jmp .loop
.end:    
    pop rbx
    pop rax
    pop rcx
    mov rsp,rbp
    pop rbp
    ret
