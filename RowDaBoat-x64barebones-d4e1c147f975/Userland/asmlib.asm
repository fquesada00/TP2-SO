GLOBAL syswrite
GLOBAL sysread
GLOBAL strlen
GLOBAL numlen
GLOBAL inforeg
GLOBAL printmem
GLOBAL processorModel
GLOBAL processorName

EXTERN printf

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

;REHACER
; -----------------------------------------------------------------------------
inforeg:
    push rbp
    mov rbp,rsp
    pushState
    mov rax,3 ;id syscall registers
    int 80h
    popState
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;Params
;   rdi -> dir
;Ret
;   -
printmem:
    push rbp
    mov rbp,rsp
    push rbx
    push rdi
.loop:
    cmp rdi,0 
    je .end ;if null, then finish
    call printf
    add rdi,0x20 ;32 bytes move, no sera 32 bits -> 4 bytes ?????¡¡¡¡¡
    jmp .loop
.end:
    pop rdi
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


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


; -----------------------------------------------------------------------------
processorName:
    push rbp
    mov rbp,rsp
    push rax
    push rbx
    push rdx
    push rcx
    push rdi ;no se si es necesario
    mov rax,0x80000002 ;extended processor name string
    cpuid
    mov [rdi],ebx
    mov [rdi + 8],edx
    mov [rdi + 16],ecx
    call printf
    mov rax,0x80000003 ;extended processor name string
    cpuid
    mov [rdi],ebx
    mov [rdi + 8],edx
    mov [rdi + 16],ecx
    call printf
    mov rax,0x80000004 ;extended processor name string
    cpuid
    mov [rdi],ebx
    mov [rdi + 8],edx
    mov [rdi + 16],ecx
    call printf
    pop rdi
    pop rcx
    pop rdx
    pop rbx
    pop rax
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
processorModel:
    push rbp
    mov rbp,rsp
    push rax
    push rbx
    push rdx
    push rcx
    push rdi ;no se si es necesario
    mov rax,1 ;processor type , model , stepping info
    cpuid
    add rax,1 ;add 1 byte
    mov [rdi],rax ;processor model
    call printf
    pop rdi
    pop rcx
    pop rdx
    pop rbx
    pop rax
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------
