
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _syscallHandler
GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN screenNumber
EXTERN get_buffer
EXTERN putsN ;TODO
EXTERN printreg
EXTERN putChar
EXTERN puts
EXTERN syscall_read
EXTERN syscall_write
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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
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
	call exceptionDispatcher

	popState
	iretq
%endmacro

;no se si se puede poner extern en una macro
%macro regDispatcher 2
    push rdi
    push rsi
	mov rdi, %1 ;pasaje de parametro 1 -> # reg
	mov rsi, %2 ;pasaje de parametro 2 -> value of # reg
    call printreg
    pop rsi
    pop rdi
%endmacro

_hlt:
	sti
	hlt
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
	irqHandlerMaster 0

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
	push rbp
	mov rbp,rsp
	cmp rax,0 ;syscall read
	je .read
	cmp rax,1 ;syscall write
	je .write
	cmp rax,2 ;syscall screen
	je .screen
	cmp rax,3 ;syscall registers
	je .register
	jmp .end
.read:
	call syscall_read
	jmp .end
.write: 
	call syscall_write
	jmp .end
.screen:
	call syscall_screen
	jmp .end
.register:
	call syscall_registers
	jmp .end
.end:
	mov rsp,rbp
	pop rbp
	ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;SYSCALL READ -> #0
;Params
;	rdi -> 0 standard input
;	rsi -> buffer to copy n bytes from standard input
;	rdx -> n bytes to read from standard input
;Ret
;	rax -> total bytes read
;syscall_read:
;	push rbp
;	mov rbp,rsp
;	push rbx
;	mov rbx,0 ;acumulador
;	cmp rdx,0
;	jle .end;if 0 bytes, finish
;.loop:
;	call is_buffer_empty
;	cmp rax,1
;	je .wait
;.continue:
;	push rsi
;	push rdx
;	call get_buffer ;rax with one byte from buffer
;	pop rdx
;	pop rsi
;	cmp rax,0
;	je .end;if char is null, finish
;	cmp rbx,rdx ;then check bytes left are diff to 0
;	je .end
;	mov byte [rsi], al ;CHEQUEAR ;copy byte into buffer
;	inc rsi
;	inc rbx
;	jmp .loop
;
;.wait:
;	call haltcpu
;	jmp .continue
;
;.end:
;	mov rax,rbx ;charge bytes read
;	pop rbx
;	mov rsp,rbp
;	pop rbp
;	ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;SYSCALL WRITE -> #1 
;Params
;	rdi -> 1 standard output
;	rsi -> buffer to read n bytes
;	rdx -> n bytes to read from buffer
;Ret
;	rax -> total bytes written
;syscall_write:
;	push rbp
;	mov rbp,rsp
;	push rbx
;	push rdi
;	mov rbx,0 ;acumulador
;	cmp rdx,0 
;	jle .end;if 0 bytes, finish
;	mov rdi,rsi
;.loop:
;	cmp byte[rsi],0
;	je .end;if null, finish
;	cmp rbx,rdx ;then check bytes left are diff to 0
;	je .end
;	push rsi
;	push rdx
;	mov rdi,0
;	mov di,[rsi]
;	call putChar ;print to STDOUT
;	pop rdx
;	pop rsi
;	inc rsi
;	inc rbx
;	jmp .loop
;.end:
;	mov rax,rbx
;	pop rdi
;	pop rbx
;	mov rsp,rbp
;	pop rbp
;	ret
;syscall_write:
;	push rbp
;	mov rbp,rsp
;	mov rdi,rsi
;	mov rsi,rdx
;	call putsN
;	mov rsp,rbp
;	pop rbp
;	ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;SYSCALL SCREEN_NUMBER -> #2
;Params
;	-
;Ret
;	rax -> screen number
syscall_screen:
	push rbp
	mov rbp,rsp
	call screenNumber
	pop rbp
	mov rsp,rbp
	ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
;SYSCALL PRINT_REGS -> #3
;Params
;	-
;Ret
;	-
syscall_registers:
	push rbp
	mov rbp,rsp
	push rax
	push rbx
	lea rbx,[rbp + 8*19] ;rbx -> rip
	mov rax,16 ;acumulador
.loop:
	cmp rax,0
	je .end ;no regs left
	regDispatcher rax, [rbx]
	sub rbx,8
	dec rax
	jmp .loop
.end:
	pop rbx
	pop rax
	mov rsp,rbp
	pop rbp
	ret
; -----------------------------------------------------------------------------

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Op Code Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret