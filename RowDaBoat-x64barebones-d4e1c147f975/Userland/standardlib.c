#include <stdint.h>
#include <stdarg.h>
#include "standardlib.h"

static char buffer[64] = {'0'};

//aca no puse el int fd como 1er parametro -> PREGUNTAR
extern int syswrite(const char * buff, int bytes); 
extern int strlen(char*);

/* return 1 if s iS greater than v
** 0 if s is equal to v
** -1 if s is lower than v */
int strcmp(const char * s, const char * v){
    for( ; s && v ; s++, v++){
        if(*s > *v) return 1;
        else if(*s < *v) return -1;
    }
    if(s && !v) return 1;
    else if(!s && v) return -1;
    return 0;
}

// variable params
void printf(const char * fmt, ...){
    va_list arg_list;
    va_start(arg_list, fmt);
    int i = 0, start, flagPercentage = 0;
    
    while(fmt[i]){
        if(!flagPercentage) start = i;//% %d%
        while(fmt[i] && (fmt[i]!='%' || flagPercentage)){
            i++;
            flagPercentage = 0;
        }
        if(i!=start) syswrite(fmt + start, (i-start) * sizeof(char));
        if(fmt[i] == '%'){
            switch (fmt[i+1])
            {
            case 'd':
                int number = va_arg(arg_list, int);
                int lenght = uintToBase(number, buffer, 10);
                syswrite(buffer, lenght * sizeof(char));
                i+=2;
                break;
            case 'c':
                char auxChar[2]={0};
                auxChar[0]=va_arg(arg_list, char);
                syswrite(auxChar, 2 * sizeof(char));
                i+=2;
                break;
            //TODO
            case 'f':

                i+=2;
                break;
            case 's':
                char * auxPointer = va_arg(arg_list, char *);
                syswrite(auxPointer,  strlen(auxPointer) * sizeof(char));
                i+=2;
                break;
            default:
                flagPercentage = 1;
                break;
            }
        }
    }
    va_end(arg_list);
}

static int uintToBase(uint64_t value, char * buffer, uint32_t base)
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


void processorInfo(){

}