#include <stdint.h>
#include <stdarg.h>
#include "standardlib.h"
#include <stdint.h>
#define MAX_PARAM 32

// asumimos fd=1 STDOUT
extern int syswrite(int fd, const char *buff, int bytes);
// asumimos fd=0 STDIN
extern int sysread(int fd, char *buff, int bytes);
extern int strlen(char *);
extern int numlen(int);
extern void printmem(long int);
extern void inforeg(void);
extern char *processorName(char*);
extern char *processorExtendedName(char *);
extern int processorModel(void);
extern int processorFamily(void);
extern int sys_GetScreen();

/* return 1 if s is greater than v
** 0 if s is equal to v
** -1 if s is lower than v */
int strcmp(const char *s, const char *v)
{
    int i = 0;
    for (; s[i] != '\0' && v[i] != '\0'; i++)
    {
        if (s[i] > v[i])
            return 1;
        else if (s[i] < v[i])
            return -1;
    }
    if (s[i] == '\0' && v[i] != '\0')
        return -1;
    else if (s[i] != '\0' && v[i] == '\0')
        return 1;
    return 0;
}

void printf(const char *fmt, ...)
{
    char auxChar[2] = {0};
    char buffer[256]={0};
    char *auxPointer;
    double num;
    va_list arg_param;
    va_start(arg_param, fmt);
    int i = 0, start = 0, numLenght, val, negative = 0;
    char auxbuff[64];
    while (fmt[i])
    {
        if (fmt[i] != '%')
            i++;
        else
        {
            if (i != start)
                syswrite(1, fmt + start, i - start);
            switch (fmt[i + 1])
            {
            case 'd':
                negative = 0;
                val = va_arg(arg_param, int);
                if (val < 0)
                {
                    negative = 1;
                    val = -val;
                    auxbuff[0] = '-';
                }
                numLenght = uintToBase(val, auxbuff + negative, 10);
                syswrite(1, auxbuff, numLenght + negative);
                i += 2;
                start = i;
                break;
            case 'c':
                auxChar[0] = va_arg(arg_param, int);
                syswrite(1, auxChar, 1);
                i += 2;
                start = i;
                break;
            case 'f':
                num = va_arg(arg_param, double);
                numLenght = doubleToString(num, buffer);
                syswrite(1, buffer, numLenght * sizeof(char));
                i += 2;
                start = i;
                break;
            case 's':
                auxPointer = va_arg(arg_param, char *);
                syswrite(1, auxPointer, strlen(auxPointer) * sizeof(char));
                i += 2;
                start = i;
                break;
            default:
                start = i++;
                break;
            }
        }
    }
    if (i != start)
        syswrite(1, fmt + start, i - start);
    va_end(arg_param);
}

int scanf(const char *fmt, ...)
{
    va_list arg_param;
    va_start(arg_param, fmt);
    int i = 0, start = 0, numLenght, j = 0;
    char auxChar[2] = {0};
    char *auxPointer;
    double num,aux=0;
    double n;
    char c;
    char buff[256]={0};
    int decimal,reset=0;
    int negative, number, idx = 0;
    int * int_param[MAX_PARAM] = {(void *) 0};
    char * char_param[MAX_PARAM] = {(void *) 0};
    char * string_param[MAX_PARAM] = {(void *) 0};
    double * double_param[MAX_PARAM] = {(void *) 0};
    int int_param_idx = 0, char_param_idx = 0, string_param_idx = 0, double_param_idx = 0;
    while (fmt[i])//hola k ->
    {
        if (fmt[i] != '%')
        {
            c = getchar();
            if (c == '\b')
            {
                if(i>=2 && fmt[i-2]=='%'){
                    switch (fmt[i-1])
                    {
                    case 'c':
                        char_param_idx--;
                        break;
                    case 'd':
                        int_param_idx--;
                        break;
                    default:
                        break;
                    }
                    reset=1;
                    i-=2;
                    putchar(c);
                }
                else if (i > 0)
                {
                    i--;
                    putchar(c);
                }
                continue;
            }
            else if (c != fmt[i])
            {
                putchar(c);
                printf("\nERROR: tipeo cualquier cosa\n");
                return;
            }
            putchar(c);
            i++;
        }
        else
        {
            c = getchar();
            if (c == '\b')
            {
                if (i > 0)
                {
                    i--;
                    putchar(c);
                }
                continue;
            }
            switch (fmt[i + 1])
            {
            case 'c':
                putchar(c);
                if(!reset) char_param[char_param_idx] = (char *) va_arg(arg_param, char *);
                *char_param[char_param_idx++] = c;
                reset = 0;
                i += 2;
                break;
            case 'd':
                negative = 0;
                number = 0;
                idx = 0;
                while ((c >= '0' && c <= '9') || c == '-' || c=='\b')
                {
                    while(c=='\b'){
                        if(idx>0){
                            idx--;
                            putchar(c);
                        }
                    }
                    if(idx>0 && c == '\b'){
                        putchar(c);
                        idx--;
                        if(negative && idx==1){
                            negative = 0;
                        }
                        else{
                            number/=10;
                        }
                        c = getchar();
                        continue;
                    }
                    else if(c=='\b'){
                        reset = 1;
                        i--;
                        break;
                    }
                    putchar(c);
                    idx++;
                    if (c == '-' && idx == 1){
                        negative = 1;
                        continue;
                    }
                    else if(c == '-'){
                        printf("\nERROR: tipeo cualquier cosa\n");
                        return;
                    }
                    else
                    {
                        number *= 10;
                        number += (c - '0');
                    }
                    c = getchar();
                }
                if(reset && idx==0) break;
                if (negative)
                    number *= -1;
                
                if(!reset){
                    
                    int_param[int_param_idx] = (int *) va_arg(arg_param, int *); 
                } 
                reset = 0;
                if (fmt[i + 2] == 0)
                {
                    *int_param[int_param_idx++] = number;
                    return;
                }
                if (fmt[i + 2] != c)
                {
                    putchar(c);
                    printf("\nERROR: tipeo cualquier cosa\n");
                    *int_param[int_param_idx++] = number;
                    return;
                } //ERROR
                *int_param[int_param_idx++] = number;
                putchar(c);
                i += 3;
                start = i;
                printf("reset");
                break;
            case 'f':
                negative = 0;
                idx = 0;
                n = 0;
                aux = 0;
                decimal = 0;
                while ((c >= '0' && c <= '9') || (c == '-') || (c == '.'))
                {
                    putchar(c);
                    idx++;
                    if (c == '-')
                    {
                        if (idx == 1){
                            negative = 1;
                            c = getchar();
                            continue;
                        }
                        else
                            break;
                    }
                    if (c == '.')
                    {
                        
                        if (decimal == 0)
                        {
                            decimal = 1;
                        }
                        else
                            break;
                    }
                    else if (!decimal)
                    {
                        n *= 10;
                        n += (c - '0');
                        
                    }
                    else
                    {
                        decimal*=10;
                        aux=(c - '0');
                        n+=aux/(double)decimal;
                    }
                    c = getchar();
                }
                if (negative)
                    n *= -1;
                if (fmt[i + 2] == 0)
                {
                    *((double *)va_arg(arg_param, double *)) = n;
                    return;
                }
                if (fmt[i + 2] != c)
                {
                    putchar(c);
                    printf("\nERROR: tipeo cualquier cosa\n");
                    *((double *)va_arg(arg_param, double *)) = n;
                    return;
                }

                *((double *)va_arg(arg_param, double *)) = n;
                putchar(c);
                i += 3;
                start = i;
                break;
            case 's':
                j = 0;
                auxPointer=va_arg(arg_param, char *);
                while ((c != ' ' && c != '\n'))
                {
                    auxPointer[j++] = c;
                    putchar(c);
                    c = getchar();
                }
                auxPointer[j] = '\0';
                if (fmt[i + 2] == 0)
                {
                    return;
                }
                putchar(c);
                i += 3;
                start = i;
                break;
            default:
                start = i++;
                break;
            }
        }
    }
    /*if(i!=start) sysread(0, buffer + start, i - start);*/
    return 0;
}

/*
int readNumber(){
    char c;
    int negative = 0, firstChar = 1, number = 0;
    while(((c = getchar())>='0' && c<='9') || (c=='-' && firstChar)){
        if(c=='-') negative = 1;
        else{
            number *= 10;
            number += (c - '0');
        }
    }
    if(negative) number *= -1;
    return number;
}
*/

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

int doubleToString(double num, char *buffer)
{   
    int neg = 0;
    char *p = buffer, *p1, *p2;
    int digits = 0;
    if(num < 0){
        neg=1;
        num=-num;
        *p++ ='-';
        digits++;
    }
    int integer_part = num;
    double decimal_part = num - integer_part;

    int aux;

    do
    {
        int remainder = integer_part % 10;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (integer_part /= 10);
    *p = '.';
    p1 = buffer+neg;
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
    for (int i = 0; i < 5; i++)
    {
        decimal_part *= 10;
        aux = decimal_part;
        decimal_part -= aux;
        *p++ = (aux < 10) ? aux + '0' : aux + 'A' - 10;
        digits++;
    }
    *p = 0;
    return digits;
}

int uintToBase(uint64_t value, char *buffer, uint32_t base)
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

void processorInfo()
{
    char buffer[256] = {0};
    processorName(buffer);
    printf("Marca del procesador: %s\n", buffer);
    processorExtendedName(buffer);
    printf("Marca del procesador extendida: %s\n", buffer);
    printf("Familia del procesador: %d\n",processorFamily());
    printf("Modelo de procesador: %d\n",processorModel());
}

void printMemoryFromAddress(long int address)
{
    char *p = (char *)address;
    for (int i = 0; i < 32; i++)
    {
        printf("%d = %d\n", p, *p++);
    }
}

void printRegisters()
{
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

int getScreen()
{
    return sys_GetScreen();
}

char getchar()
{
    char buffer[2] = {0};
    sysread(0, buffer, 1);
    return buffer[0];
}

void putchar(char c)
{
    char buff[2];
    buff[0] = c;
    buff[1] = 0;
    syswrite(1, buff, 1);
}





/*
int n1,n2
char c

if((scanf(%f)==1 && scanf( %c )==1 && scanf(%f)==1) || 
(scanf(%c)==1 && scanf( %f )==1 && scanf(%c)==1 && scanf(%f)==1) || (scanf(%c)==1))
(2+3)

(E op E)
E=n
*/