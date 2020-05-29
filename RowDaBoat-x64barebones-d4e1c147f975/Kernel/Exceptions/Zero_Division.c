#include "Zero_Division.h"

extern void registers(void);

//NO VA ACA Y ESTA MAL
void printf(char* s,...){
    return;
}

void zero_division_handler(){
    registers();
}

void printreg(int reg, int val){
    switch (reg){
    case 16:
        printf("rip=");
        break;
    case 15:
        printf("rax=");
        break;
    case 14:
        printf("rbx=");
        break;
    case 13:
        printf("rcx=");
        break;
    case 12:
        printf("rdx=");
        break;
    case 11:
        printf("rbp=");
        break;
    case 10:
        printf("rdi=");
        break;
    case 9:
        printf("rsi=");
        break;
    case 8:
        printf("r8=");
        break;
    case 7:
        printf("r9=");
        break;
    case 6:
        printf("r10=");
        break;
    case 5:
        printf("r11=");
        break;
    case 4:
        printf("r12=");
        break;
    case 3:
        printf("r13=");
        break;
    case 2:
        printf("r14=");
        break;
    case 1:
        printf("r15=");
        break;
    default:
        break;
    }

    printf("%d",val);

}