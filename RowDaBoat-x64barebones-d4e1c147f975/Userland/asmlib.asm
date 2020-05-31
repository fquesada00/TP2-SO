GLOBAL syswrite
GLOBAL strlen
GLOBAL numlen

SECTION .text


; -----------------------------------------------------------------------------
;int syswrite(int fd (en 1 por default), const char * buff, int bytes)
syswrite:
    push rbp
    mov rbp,rsp
    mov rax,1 ;id syscall write
    int 80h
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;int sysread(int fd (en 0 por default), const char * buff, int bytes)
sysread:
    push rbp
    mov rbp,rsp
    mov rax,0 ;id syscall read
    int 80h
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;Params
;   rdi -> char * string
;Ret
;   rax -> string lenght
strlen:
    push rbp
    mov rbp,rsp
    push rdi
    mov rax,0
.loop:
    cmp byte [rdi],0
    je .end ;if null, then finish
    inc rax
    inc rdi
    jmp .loop
.end:
    pop rdi
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;Params
;   rdi -> int n
;Ret
;   rax -> n lenght
numlen:
    push rbp
    mov rbp,rsp
    push rbx
    push rcx
    push rdx
    mov rbx,0 ;accum
    cmp rdi,0 ;if null, finish
    je .end
    mov rax,[rdi] ;charge number
.loop:
    mov rcx,10 ;divisor
    mov rdx,0 ;resto
    div rcx ;rax divided by rcx
    inc rbx 
    cmp rax,0
    je .end ;if 0, then number is gone
    jmp .loop
.end:
    mov rax,rbx
    pop rdx
    pop rcx
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------
