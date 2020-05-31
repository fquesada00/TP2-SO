#include <stdint.h>
#include <stdarg.h>
#include "standardlib.h"

static char buffer[64] = {'0'};

// asumimos fd=1 STDOUT
extern int syswrite(int fd, const char * buff, int bytes); 
// asumimos fd=0 STDIN
extern int sysread(int fd, char * buff, int bytes); 
extern int strlen(char*);
extern void printmem(long int);
extern void inforeg(void);
extern void processorName(void);
extern void processorModel(void);

/* return 1 if s is greater than v
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
    int pEntera = 0, pDecimal = 0;
    while(fmt[i]){
        if(!flagPercentage) start = i;//% %d%
        while(fmt[i] && (fmt[i]!='%' || flagPercentage)){
            i++;
            flagPercentage = 0;
        }
        if(i!=start) syswrite(1, fmt + start, (i-start) * sizeof(char));
        if(fmt[i] == '%'){
            if(!fmt[i+1] && fmt[i+1]!='d' && fmt[i+1]!='c' && fmt[i+1]!='s'){
                if(fmt[i+1]!='f'){ //then is . or a number
                    if(fmt[i+1]=='.'){
                        pDecimal; //hay que hacer un strToNum
                    }
                }
            }
            switch (fmt[i+1])
            {
            case 'd':
                int number = va_arg(arg_list, int);
                int lenght = uintToBase(number, buffer, 10);
                syswrite(1, buffer, lenght * sizeof(char));
                i+=2;
                break;
            case 'c':
                char auxChar[2] = {0};
                auxChar[0] = va_arg(arg_list, char);
                syswrite(1, auxChar, 2 * sizeof(char));
                i+=2;
                break;
            //TODO
            case 'f':
                i+=2;
                break;
            case 's':
                char * auxPointer = va_arg(arg_list, char *);
                syswrite(1, auxPointer,  strlen(auxPointer) * sizeof(char));
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
    printf("Marca/Nombre del procesador extendido: ");
    processorName();
    printf("Modelo de procesador: ");
    processorModel();
}


void printMemoryFromAddress(long int address){
    printmem(address);
}


void printRegisters(){
    inforeg();
}