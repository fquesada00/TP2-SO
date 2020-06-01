#include <stdint.h>
#include <stdarg.h>
#include "standardlib.h"
#include <stdint.h>
static char buffer[256] = {'0'};

// asumimos fd=1 STDOUT
extern int syswrite(int fd, const char * buff, int bytes); 
// asumimos fd=0 STDIN
extern int sysread(int fd, char * buff, int bytes); 
extern int strlen(char*);
extern int numlen(int);
extern void printmem(long int);
extern void inforeg(void);
extern char * processorName(char*);
extern int processorModel(void);

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

void printf(const char * fmt,...)
{
    char auxChar[2]={0};
    char * auxPointer;
    double num;
    va_list arg_param;
    va_start(arg_param,fmt);
    int i = 0,start=0,numLenght;
    char auxbuff[64];
    while (fmt[i])
    {
        if(fmt[i]!='%')
            i++;
        else
        {
            if(i!=start)
                syswrite(1,fmt+start,i-start);
            switch (fmt[i+1])
            {
            case 'd':
                numLenght= uintToBase(va_arg(arg_param,int),auxbuff,10);
                syswrite(1,auxbuff,numLenght);
                i+=2;
                start = i;
                break;
            case 'c':
                auxChar[0] = va_arg(arg_param, int);
                syswrite(1, auxChar, 2 * sizeof(int));
                i+=2;
                start = i;
                break;
            case 'f':
                num = va_arg(arg_param,double);
                numLenght = doubleToString(num,buffer);
                syswrite(1,buffer,numLenght * sizeof(char));
                i+=2;
                start = i;
                break;
            case 's':
                auxPointer = va_arg(arg_param, char *);
                syswrite(1, auxPointer,  strlen(auxPointer) * sizeof(char));
                i+=2;
                start = i;
                break;
            default:
                start = i++;
                break;

            }

        }
    }
    if(i!=start) syswrite(1,fmt+start,i-start);
    va_end(arg_param);
}

void scanf(const char * fmt, ...){
    va_list arg_param;
    va_start(arg_param,fmt);
    int i = 0, start = 0;
    while(fmt[i]){
        if(fmt[i]!='%'){
            i++;
        }
        else{
            if(i!=start){
                sysread(0, buffer + start, i - start);
            }
            switch (fmt[i+1])
            {
            case 'd':
                
                break;
            
            default:
                start = i++;
                break;
            }

        }
    }
    if(i!=start) sysread(0, buffer + start, i - start);
}



// variable params
/*
void printf(const char * fmt, ...){
    va_list arg_list;
    va_start(arg_list, fmt);
    int i = 0, start, flagPercentage = 0;
    int pEntera = 0, pDecimal = 0;
    int number,lenght;
    char auxChar[2]={0};
    char * auxPointer;
    double num;
    while(fmt[i]){ //%%
        if(!flagPercentage) start = i;
        while(fmt[i] && (fmt[i]!='%' || flagPercentage)){
            i++;
            flagPercentage = 0;
        }
        if(i!=start) syswrite(1, fmt + start, (i-start) * sizeof(char));
        if(fmt[i] == '%'){
            switch (fmt[i+1])
            {
            case 'd':
                number = va_arg(arg_list, int);
                lenght = uintToBase(number, buffer, 10);
                syswrite(1, buffer, lenght * sizeof(char));
                i+=2;
                break;
            case 'c':
                auxChar[0] = va_arg(arg_list, int);
                syswrite(1, auxChar, 2 * sizeof(int));
                i+=2;
                break;
            //TODO
            case 'f':
                num = va_arg(arg_list,double);
                lenght = doubleToString(num,buffer);
                syswrite(1,buffer,lenght * sizeof(char));
                i+=2;
                break;
            case 's':
                auxPointer = va_arg(arg_list, char *);
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
*/

int doubleToString(double num, char * buffer)
{
    int integer_part = num;
    double decimal_part = num - integer_part;
    int digits = 0;
    char * p = buffer,* p1,* p2;
    int aux;
    do{
        int remainder = integer_part % 10;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
    }while(integer_part/=10);
    *p='.';
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
    p++;
    for(int i = 0;i < 5;i++)
    {
        decimal_part*=10;
        aux=decimal_part;
        decimal_part-=aux;
        *p++ = (aux < 10) ? aux + '0' : aux + 'A' - 10;
		digits++;
    }
    *p=0;
    return digits;
}

int uintToBase(uint64_t value, char * buffer, uint32_t base)
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
    processorName(buffer);
    printf("Marca del procesador: %s\n", buffer);
    printf("Modelo de procesador: %d\n", processorModel());
}


void printMemoryFromAddress(long int address){
    printmem(address);
}


void printRegisters(){
    inforeg();
}


/*
char * intToStr(int n){
    int lenght = numlen(n);
    char * p = bufferInt;
    while (lenght)
    {
        int remainder = n % 10;
        p[--lenght] = remainder + '0';
        n/=10;
    }
    return p;
}*/