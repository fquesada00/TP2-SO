GLOBAL syswrite
GLOBAL sysread
GLOBAL divExc
GLOBAL invalidOpCode
GLOBAL read_mem
GLOBAL _execv
GLOBAL pMalloc
GLOBAL pFree
GLOBAL pKill
GLOBAL printProcesses
GLOBAL pBlock
GLOBAL _getPID
GLOBAL _nice
GLOBAL pipeClose
GLOBAL pipeOpen
GLOBAL initProcessWithPipe
GLOBAL semClose
GLOBAL semOpen
GLOBAL semPost
GLOBAL semWait
GLOBAL semPrint
GLOBAL _waitPID
GLOBAL _yield
GLOBAL _exit
GLOBAL pipePrint
GLOBAL flushFD
GLOBAL syscall_mem

SECTION .text

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
invalidOpCode:
    push rbp
    mov rbp,rsp
    UD2
    mov rsp,rbp
    pop rbp
    ret
    ; -----------------------------------------------------------------------------
syscall_mem:
    push rbp
    mov rbp,rsp
    mov rax,6
    int 80h
    mov rsp,rbp
    pop rbp
    ret

pMalloc:
    push rbp
    mov rbp,rsp
    mov rax,9
    int 80h
    mov rsp,rbp
    pop rbp
    ret
pFree:
    push rbp
    mov rbp,rsp
    mov rax,10
    int 80h
    mov rsp,rbp
    pop rbp
    ret

_execv:
    push rbp
    mov rbp,rsp
    mov rax,11
    int 80h
    mov rsp,rbp
    pop rbp
    ret

pKill:
    push rbp
    mov rbp,rsp
    mov rax,7
    int 80h
    mov rsp,rbp
    pop rbp
    ret
printProcesses:
    push rbp
    mov rbp,rsp
    mov rax,8
    int 80h
    mov rsp,rbp
    pop rbp
    ret

pBlock:
    push rbp
    mov rbp,rsp
    mov rax,12
    int 80h
    mov rsp,rbp
    pop rbp
    ret
_getPID:
    push rbp
    mov rbp,rsp
    mov rax,13
    int 80h
    mov rsp,rbp
    pop rbp
    ret
_nice:
    push rbp
    mov rbp,rsp
    mov rax,14
    int 80h
    mov rsp,rbp
    pop rbp
    ret

semOpen:
    push rbp
    mov rbp, rsp
    mov rax,15
    int 80h
    mov rsp,rbp
    pop rbp
    ret

semWait:
    push rbp
    mov rbp,rsp
    mov rax,16
    int 80h
    mov rsp,rbp
    pop rbp
    ret

semPost:
    push rbp
    mov rbp,rsp
    mov rax,17
    int 80h
    mov rsp,rbp
    pop rbp
    ret

semClose:
    push rbp
    mov rbp,rsp
    mov rax,18
    int 80h
    mov rsp,rbp
    pop rbp
    ret

semPrint:
    push rbp
    mov rbp,rsp
    mov rax,19
    int 80h
    mov rsp,rbp
    pop rbp
    ret

pipeOpen:
    push rbp
    mov rbp,rsp
    mov rax,20
    int 80h
    mov rsp,rbp
    pop rbp
    ret

pipeClose:
    push rbp
    mov rbp,rsp
    mov rax,21
    int 80h
    mov rsp,rbp
    pop rbp
    ret

initProcessWithPipe:
    push rbp
    mov rbp,rsp
    mov rax,22
    int 80h
    mov rsp,rbp
    pop rbp
    ret

_waitPID:
    push rbp
    mov rbp,rsp
    mov rax,23
    int 80h
    mov rsp,rbp
    pop rbp
    ret
_yield:
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
pipePrint:
    push rbp
    mov rbp,rsp
    mov rax,26
    int 80h
    mov rsp,rbp
    pop rbp
    ret

flushFD:
    push rbp
    mov rbp,rsp
    mov rax,27
    int 80h
    mov rsp,rbp
    pop rbp
    ret