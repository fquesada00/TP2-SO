GLOBAL syswrite
GLOBAL sysread
GLOBAL processorTemperature
GLOBAL inforeg
GLOBAL processorModel
GLOBAL processorName
GLOBAL processorExtendedName
GLOBAL processorFamily
GLOBAL divExc
GLOBAL loadPrgrm
GLOBAL sysrtc
GLOBAL invalidOpCode
GLOBAL read_mem
GLOBAL sys_execv
GLOBAL syscallMalloc
GLOBAL syscallFree
GLOBAL syscallKill
GLOBAL syscallProcesses
GLOBAL syscallBlock
GLOBAL getPID
GLOBAL nice
GLOBAL syscallPipeClose
GLOBAL syscallPipeOpen
GLOBAL syscallInitProcessWithPipe
GLOBAL syscallSemClose
GLOBAL syscallSemOpen
GLOBAL syscallSemPost
GLOBAL syscallSemWait
GLOBAL syscallSemPrint
GLOBAL waitPID
GLOBAL yield
GLOBAL _exit

SECTION .text

; REHACER
; -----------------------------------------------------------------------------
inforeg:
    push rbp
    mov rbp,rsp
    mov rax,2          ; id syscall registers
    int 80h
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------


    ; -----------------------------------------------------------------------------
    ; int syswrite(int fd (en 1 por default), const char * buff, int bytes)
syswrite:
    push rbp
    mov rbp,rsp
    mov rax,1          ; id syscall write
    int 80h
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------


    ; -----------------------------------------------------------------------------
    ; int sysread(int fd (en 0 por default), const char * buff, int bytes)
sysread:
    push rbp
    mov rbp,rsp
    mov rax,0          ; id syscall read
    int 80h
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------


    ; -----------------------------------------------------------------------------
    ; int sysrtc(int n)
sysrtc:
    push rbp
    mov rbp,rsp
    mov rax,3          ; id syscall rtc
    int 80h
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------

    ; -----------------------------------------------------------------------------
    ; Params
    ; rdi -> char * buffer
    ; Ret
    ; -
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
    ; Params
    ; rdi -> char * buffer
    ; Ret
    ; -
processorExtendedName:
    push rbp
    mov rbp,rsp
    push rbx
    push rax
    mov rax,0x80000002 ; extended processor name string
    cpuid
    mov [rdi],eax
    mov [rdi + 4],ebx
    mov [rdi + 8],ecx
    mov [rdi + 12],edx
    mov rax,0x80000003 ; extended processor name string
    cpuid
    mov [rdi + 16],eax
    mov [rdi + 20],ebx
    mov [rdi + 24],edx
    mov [rdi + 28],ecx
    mov rax,0x80000004 ; extended processor name string
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
    ; Params
    ; -
    ; Ret
    ; rax -> int processor model
processorModel:
    push rbp
    mov rbp,rsp
    push rbx
    mov rax,1
    cpuid
    shr rax,4
    and rax,0x0F       ; first 4 bits -> processor model
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------


    ; -----------------------------------------------------------------------------
    ; Params
    ; -
    ; Ret
    ; rax -> int processor family
processorFamily:
    push rbp
    mov rbp,rsp
    push rbx
    mov rax,1
    cpuid
    shr rax,8
    and rax,0x0F
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------

loadPrgrm:
    push rbp
    mov rbp,rsp
    mov rax,4
    int 80h
    mov rsp,rbp
    pop rbp
    ret

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
processorTemperature:
    push rbp
    mov rbp,rsp
    mov rax,5
    int 80h
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------
invalidOpCode:
    push rbp
    mov rbp,rsp
    UD2
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------
read_mem:
    push rbp
    mov rbp,rsp
    mov rax,6
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallMalloc:
    push rbp
    mov rbp,rsp
    mov rax,9
    int 80h
    mov rsp,rbp
    pop rbp
    ret
syscallFree:
    push rbp
    mov rbp,rsp
    mov rax,10
    int 80h
    mov rsp,rbp
    pop rbp
    ret

sys_execv:
    push rbp
    mov rbp,rsp
    mov rax,11
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallKill:
    push rbp
    mov rbp,rsp
    mov rax,7
    int 80h
    mov rsp,rbp
    pop rbp
    ret
syscallProcesses:
    push rbp
    mov rbp,rsp
    mov rax,8
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallBlock:
    push rbp
    mov rbp,rsp
    mov rax,12
    int 80h
    mov rsp,rbp
    pop rbp
    ret
getPID:
    push rbp
    mov rbp,rsp
    mov rax,13
    int 80h
    mov rsp,rbp
    pop rbp
    ret
nice:
    push rbp
    mov rbp,rsp
    mov rax,14
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallSemOpen:
    push rbp
    mov rbp, rsp
    mov rax,15
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallSemWait:
    push rbp
    mov rbp,rsp
    mov rax,16
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallSemPost:
    push rbp
    mov rbp,rsp
    mov rax,17
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallSemClose:
    push rbp
    mov rbp,rsp
    mov rax,18
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallSemPrint:
    push rbp
    mov rbp,rsp
    mov rax,19
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallPipeOpen:
    push rbp
    mov rbp,rsp
    mov rax,20
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallPipeClose:
    push rbp
    mov rbp,rsp
    mov rax,21
    int 80h
    mov rsp,rbp
    pop rbp
    ret

syscallInitProcessWithPipe:
    push rbp
    mov rbp,rsp
    mov rax,22
    int 80h
    mov rsp,rbp
    pop rbp
    ret

waitPID:
    push rbp
    mov rbp,rsp
    mov rax,23
    int 80h
    mov rsp,rbp
    pop rbp
    ret
yield:
    push rbp
    mov rbp,rsp
    mov rax,24
    int 80h
    mov rsp,rbp
    pop rbp
    ret
_exit:
    push rbp
    mov rbp,rsp
    mov rax,25
    int 80h
    mov rsp,rbp
    pop rbp
    ret
