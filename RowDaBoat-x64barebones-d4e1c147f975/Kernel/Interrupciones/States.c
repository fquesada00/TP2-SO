#include "States.h"
static State savedState={0};
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}
void printreg(State saved)
{
    char buffer[64];
    uintToBase(saved.rax, buffer, 16);
    putsColor("rax= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rbx, buffer, 16);
    putsColor("rbx= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rcx, buffer, 16);
    putsColor("rcx= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rdx, buffer, 16);
    putsColor("rdx= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rbp, buffer, 16);
    putsColor("rbp= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rdi, buffer, 16);
    putsColor("rdi= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rsi, buffer, 16);
    putsColor("rsi= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r8, buffer, 16);
    putsColor("r8= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r9, buffer, 16);
    putsColor("r9= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r10, buffer, 16);
    putsColor("r10= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r11, buffer, 16);
    putsColor("r11= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r12, buffer, 16);
    putsColor("r12= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r13, buffer, 16);
    putsColor("r13= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r14, buffer, 16);
    putsColor("r14= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.r15, buffer, 16);
    putsColor("r15= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rip, buffer, 16);
    putsColor("rip= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
    uintToBase(saved.rsp, buffer, 16);
    putsColor("rsp= ", 255, 0, 0);
    puts(buffer);
    putChar('\n');
}
void save_regs(uint64_t rsp)
{

    
    State * currentState = (void *) rsp;
   memcpy(&savedState,currentState,sizeof(State));
}
State * getRegs()
{
    return &savedState;
}
