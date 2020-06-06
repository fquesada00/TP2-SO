GLOBAL syswrite
GLOBAL sysread
GLOBAL processorTemperature
GLOBAL numlen
GLOBAL inforeg
GLOBAL printmem
GLOBAL processorModel
GLOBAL processorName
GLOBAL processorExtendedName
GLOBAL sys_GetScreen
GLOBAL processorFamily
GLOBAL divExc
GLOBAL loadProgram
GLOBAL sysrtc

EXTERN printf
EXTERN putchar

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
    mov rax,3 ;id syscall registers
    int 80h
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
    mov rbx,0 ;acumulador
.loop:
    cmp rbx,32
    je .end
    push rdi
    mov rdi,[rdi]
    call printf
    pop rdi
    inc rdi ;next byte
    inc rbx
    jmp .loop
.end:
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
;int sysrtc(int n)
sysrtc:
    push rbp
    mov rbp,rsp
    mov rax,4 ;id syscall rtc
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
;strlen:
;    push rbp
;    mov rbp,rsp
;    push rdi
;    mov rax,0
;.loop:
;    cmp byte [rdi],0
;    je .end ;if null, then finish
;    inc rax
;    inc rdi
;    jmp .loop
;.end:
;    pop rdi
;    mov rsp,rbp
;    pop rbp
;    ret
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
    mov rax,rdi ;charge number
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
;Params
;   rdi -> char * buffer
;Ret
;   -
processorName:
    push rbp
    mov rbp,rsp
    push rax
    push rbx
    mov rax,0
    cpuid
    mov [rdi],ebx
    mov [rdi + 4],edx
    mov [rdi + 8],ecx
    mov byte [rdi + 13],0
    pop rbx
    pop rax
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;Params
;   rdi -> char * buffer
;Ret
;   -
processorExtendedName:
    push rbp
    mov rbp,rsp
    push rbx
    push rax
    mov rax,0x80000002 ;extended processor name string
    cpuid
    mov [rdi],eax
    mov [rdi + 4],ebx
    mov [rdi + 8],ecx
    mov [rdi + 12],edx
    mov rax,0x80000003 ;extended processor name string
    cpuid
    mov [rdi + 16],eax
    mov [rdi + 20],ebx
    mov [rdi + 24],edx
    mov [rdi + 28],ecx
    mov rax,0x80000004 ;extended processor name string
    cpuid
    mov [rdi + 32],eax
    mov [rdi + 36],ebx
    mov [rdi + 40],edx
    mov [rdi + 44],ecx
    mov byte [rdi + 45], 0
    pop rax
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;Params
;   -
;Ret
;   rax -> int processor model
processorModel:
    push rbp
    mov rbp,rsp
    push rbx
    mov rax,1 
    cpuid
    shr rax,4 
    and rax,0000000000000000000000000000000000000000000000000000000000001111b ;first 4 bits -> processor model
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;Params
;   -
;Ret
;   rax -> int processor family
processorFamily:
    push rbp
    mov rbp,rsp
    push rbx
    mov rax,1 
    cpuid
    shr rax,8
    and rax,0x000000000000000F
    ;and rax,0000000000000000000000000000000000000000000000000000000000001111b
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------

loadProgram:
    push rbp
    mov rbp,rsp
    mov rax,5
    int 80h
    mov rsp,rbp
    pop rbp
    ret

; -----------------------------------------------------------------------------
sys_GetScreen:
    push rbp
    mov rbp,rsp
    mov rax,2
    int 80h
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------


divExc:
    push rbp
    mov rbp,rsp
    mov rdi,0
    mov rax,0x123
    div rdi
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------
processorCriticalTemperature:
    push rbp
    mov rbp,rsp
    ;rdmsr -a IA32_THERM_STATUS

    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------
