#include "Zero_Division.h"
#include "../Video_Driver/video_driver.h"
extern void save_regs();
extern uint64_t * getRegs();

void zero_division_handler(){
    
    save_regs();
    uint64_t * buffer = getRegs();
    putsColor("ZERO DIVISION EXCEPTION\n",255,0,0);
    for(int i = 15;i>0;i--)
    {
        printreg(i,buffer[i-1]);
    }
}
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
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
	}
	while (value /= base);

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
void printreg(int reg, uint64_t val){
    char buffer[64];
    uintToBase(val,buffer,16);
    switch (reg){
    case 15:
        putsColor("rax= ",255,0,0);
        break;
    case 14:
        putsColor("rbx= ",255,0,0);
        break;
    case 13:
        putsColor("rcx= ",255,0,0);
        break;
    case 12:
        putsColor("rdx= ",255,0,0);
        break;
    case 11:
        putsColor("rbp= ",255,0,0);
        break;
    case 10:
        putsColor("rdi= ",255,0,0);
        break;
    case 9:
        putsColor("rsi= ",255,0,0);
        break;
    case 8:
        putsColor("r8= ",255,0,0);
        break;
    case 7:
        putsColor("r9= ",255,0,0);
        break;
    case 6:
        putsColor("r10= ",255,0,0);
        break;
    case 5:
        putsColor("r11= ",255,0,0);
        break;
    case 4:
        putsColor("r12= ",255,0,0);
        break;
    case 3:
        putsColor("r13= ",255,0,0);
        break;
    case 2:
        putsColor("r14= ",255,0,0);
        break;
    case 1:
        putsColor("r15= ",255,0,0);
        break;
    default:
        break;
    }
    puts(buffer);
    putChar('\n');

}