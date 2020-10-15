GLOBAL _xchg
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL _hltAndCli

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _syscallHandler
GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL initProcessManager
GLOBAL execvAuxi
GLOBAL _int20

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscall_read
EXTERN syscall_write
EXTERN syscall_registers
EXTERN loadProgram
EXTERN syscall_read_mem
EXTERN schedule
EXTERN init_process
EXTERN pMalloc
EXTERN pFree
EXTERN pKill
EXTERN processes
EXTERN blockProcess
EXTERN getPID
EXTERN nice
EXTERN sem_open
EXTERN sem_wait
EXTERN sem_post
EXTERN sem_close
EXTERN sem
EXTERN pipeOpen
EXTERN init_process_with_pipe
EXTERN pipeClose

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

%macro pushStateNoRAX 0
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

%macro popStateNoRAX 0
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
%endmacro

%macro irqHandlerMaster 1
	pushState
	mov rdi, %1 ; pasaje de parametro
	mov rsi,rsp
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState
	mov rdi, %1 ; pasaje de parametro
	mov rsi,rsp
	call exceptionDispatcher
	popState
	iretq
%endmacro

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Op Code Exception
_exception6Handler:
	exceptionHandler 6

_hlt:
	sti
	hlt
	ret

_hltAndCli:
	sti
	hlt
	cli
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	pushState
	mov rdi,rsp
	call schedule
	mov rsp,rax
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
	popState
	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


; -----------------------------------------------------------------------------
;KERNEL SYSCALL HANDLER INTERRUPT 80h										
;Params
;	rax -> syscall number
;Ret								
;	rax -> syscall asociated value					
_syscallHandler:
	pushStateNoRAX
	cmp rax,0 ;syscall read
	je .read
	cmp rax,1 ;syscall write
	je .write
	cmp rax,2 ;syscall registers
	je .register
	cmp rax,3 ;syscall rtc
	je .rtc
	cmp rax,4 ;syscall execute
	je .exe
	cmp rax,5 ;syscall tmp
	je .tmp
	cmp rax,6
	je .read_mem ;syscall read memory
	cmp rax,7
	je .kill
	cmp rax,8
	je .ps
	cmp rax,9
	je .malloc
	cmp rax,10
	je .free
	cmp rax,11
	je .execv
	cmp rax,12
	je .block
	cmp rax,13
	je .pid
	cmp rax,14
	je .nice
	cmp rax,15
	je .semaphore_open
	cmp rax,16
	je .semaphore_wait
	cmp rax,17
	je .semaphore_post
	cmp rax,18
	je .semaphore_close
	cmp rax,19
	je .semaphore_print
	cmp rax,20
	je .create_pipe
	cmp rax,21
	je .close_pipe
	cmp rax,22
	je .create_process_with_pipe
	jmp .end
.read:
	push rdi
	push rsi
	push rdx
	call syscall_read
	pop rdx
	pop rsi
	pop rdx
	jmp .end
.write: 
	push rdi
	push rsi
	push rdx
	call syscall_write
	pop rdx
	pop rsi
	pop rdx
	jmp .end
.register:
	call syscall_registers
	jmp .end
.exe:
	;call loadProgram
	jmp .end
.rtc:
	call syscall_rtc
	jmp .end
.tmp:
	call syscall_tmp
	jmp .end
.read_mem:
	mov rcx, rsp
	call syscall_read_mem
	jmp .end
.execv:
	mov rcx,rsp
	call init_process
	jmp .end
.malloc:
	call pMalloc
	jmp .end
.free:
	call pFree
	jmp .end
.kill:
	call pKill
	jmp .end
.ps:
	call processes
	jmp .end
.block:
	mov rdx, rsp
	call blockProcess
	jmp .end
.pid:
	call getPID
	jmp .end
.nice:
	call nice
	jmp .end
.semaphore_open:
	call sem_open
	jmp .end
.semaphore_wait:
	call sem_wait
	jmp .end
.semaphore_post:
	call sem_post
	jmp .end
.semaphore_close:
	call sem_close
	jmp .end
.semaphore_print:
	call sem
	jmp .end
.create_pipe:
	call pipeOpen
	jmp .end
.close_pipe:
	call pipeClose
	jmp .end
.create_process_with_pipe
	call init_process_with_pipe
	jmp .end
.end:
	popStateNoRAX
	iretq
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
;SYSCALL RTC -> #3
;Params
;	rdi -> int n
;Ret
;	rax -> time
syscall_rtc:
	push rbp
    mov rbp,rsp
    push rbx
    mov rax,0
    mov rbx,0
    mov ax,di
    out 70h,al
    in al,71h
    mov bl,al
    and bl,15
    shr al,4
    mov cl,10
    mul cl
    add al,bl
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
; -----------------------------------------------------------------------------

haltcpu:
	cli
	hlt
	ret


; -----------------------------------------------------------------------------
;SYSCALL TMP -> #5
;Ret
;	rax -> processor temperature
syscall_tmp:
	push rbp
	mov rbp,rsp
	mov rcx,0
	mov ecx,0x19C;DELTA
	;rdmsr 
	mov rax,0x88330008
	mov rdi, rax
	shr rdi,16
	and rdi,0x7F
	mov rcx,0
	mov ecx,0x1A2;TJMAX
	;rdmsr 
	mov rax,0x5640000
	mov rsi,rax
	shr rsi,16
	and rsi,0xFF
	sub rsi,rdi
	mov rax,rdi
	mov rsp,rbp
	pop rbp
	ret

initProcessManager:
	call init_process
	int 20h
	popState
	ret
execvAuxi:
	push rbp
	mov rbp,rsp
	mov rax,11
	int 80h
	mov rsp,rbp
	pop rbp
	ret

_int20:
	int 20h
	ret

;chequear
_xchg:
	xchg [rdi], rsi
	mov rax,rsi
	ret

