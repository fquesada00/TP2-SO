#include <stdint.h>
#include <stdarg.h>
#include "standardlib.h"

// asumimos fd=1 STDOUT
extern int syswrite(int fd, const char *buff, int bytes);
// asumimos fd=0 STDIN
extern int sysread(int fd, char *buff, int bytes);
extern int numlen(int);
extern void printmem(long int);
extern void inforeg(void);
extern char *processorName(char *);
extern char *processorExtendedName(char *);
extern int processorModel(void);
extern int processorFamily(void);
extern int sys_GetScreen();
extern void processorTemperature();
extern unsigned char sysrtc(int);


void puts(char * c){
    int i = 0;
    while(c[i]){
        putchar(c[i++]);
    }
    putchar('\n');
}

void vaArg(int n, ...){
    va_list params;
    va_start(params, n);
    char buff[64];
    char * aux;
    int p;
    for(int i = 0 ; i < n ; i++){
        puts(va_arg(params, char *));
    }
}

/* return 1 if s is greater than v
** 0 if s is equal to v
** -1 if s is lower than v */
int strcmp(const char *s, const char *v)
{
    int i = 0;
    for ( ; s[i] != '\0' && v[i] != '\0'; i++)
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

int strlen(char *s)
{
    int i = 0;
    while (s[i] != 0){
        i++;
    }
    return i;
}

int printf(const char * fmt, ...){
    va_list arg_param;
    va_start(arg_param, fmt);
    int idxFmt = 0, idxBuffer = 0;
    char buffer[256] = {0};
    char * sAtFmt;
    int nAtFmt, nAtFmtLenght, sAtFmtLenght, idxsAtFmt, fAtFmtLenght;
    double fAtFmt;
    while(fmt[idxFmt]){
        if(fmt[idxFmt] != '%'){
            buffer[idxBuffer++] = fmt[idxFmt++];
        }
        else{
            switch (fmt[idxFmt + 1])
            {
            case 'c':
                buffer[idxBuffer++] = (char) va_arg(arg_param, int);
                idxFmt += 2;
                break;
            case 'd':
                nAtFmt = va_arg(arg_param, int);
                if(nAtFmt < 0){
                    nAtFmt *= -1;
                    buffer[idxBuffer++] = '-';
                }
                nAtFmtLenght = uintToBase(nAtFmt, buffer + idxBuffer, 10);
                idxFmt += 2;
                idxBuffer += nAtFmtLenght;
                break;
            case 's':
                sAtFmt = va_arg(arg_param, char *);
                sAtFmtLenght = strlen(sAtFmt);
                idxsAtFmt = 0;
                while(sAtFmt[idxsAtFmt]){
                    buffer[idxBuffer++] = sAtFmt[idxsAtFmt++];
                }                
                idxFmt += 2;
                break;
            case 'f':
                fAtFmt = va_arg(arg_param, double);
                fAtFmtLenght = doubleToString(fAtFmt, buffer + idxBuffer);
                idxBuffer += fAtFmtLenght;
                idxFmt += 2;
                break;
            default:
                buffer[idxBuffer++] = fmt[idxFmt++];
                break;
            }
        }
    }
    if(idxBuffer > 0){
        syswrite(1, buffer, idxBuffer);
    }
    va_end(arg_param);
    return idxBuffer;
}

int scanf(const char *fmt, ...)
{
    va_list arg_param;
    va_start(arg_param, fmt);
    char buffer[256] = {0};
    int c, idxBuffer = 0, argsRead = 0, idxAuxPointer = 0, idxFmt = 0;
    int negative, number, idxNumber, idxFloat, decimal;
    double floatNumber;
    char * auxPointer;
    double * doublePointer;
    while ((c = getchar()) != '\n')
    {
        if (c != '\b')
        {
            buffer[idxBuffer++] = (char) c;
            putchar(c);
        }
        else if (idxBuffer > 0)
        {
            idxBuffer--;
            putchar(c);
        }
    }
    buffer[idxBuffer] = 0;
    if (idxBuffer == 0)
        return argsRead;
    idxBuffer = 0;
    while (fmt[idxFmt])
    {
        if (fmt[idxFmt] != '%')
        {
            if (buffer[idxBuffer] != fmt[idxFmt])
            {
                printf("\nERROR: tipeo cualquier cosa y te muestro:");
                printf("\nidxFmtError: %d y %d\n", idxFmt, idxBuffer);
                return -1;
            }
            idxFmt++;
            idxBuffer++;
        }
        else
        {
            switch (fmt[idxFmt + 1])
            {
            case 'c':
                *((char *)va_arg(arg_param, char *)) = buffer[idxBuffer++];
                idxFmt += 2;
                argsRead++;
                break;
            case 'd':
                negative = 0;
                number = 0;
                idxNumber = 0;
                char nAtBuffer = buffer[idxBuffer++];
                while ((nAtBuffer >= '0' && nAtBuffer <= '9') || nAtBuffer == '-')
                {
                    if (idxNumber == 0)
                    {
                        idxNumber++;
                        if (nAtBuffer == '-')
                        {
                            negative = 1;
                            nAtBuffer = buffer[idxBuffer++];
                            continue;
                        }
                        else
                            negative = 0;
                    }
                    else if (nAtBuffer == '-')
                    {
                        printf("\nERROR: Metiste un -\n");
                        return -1;
                    }
                    number *= 10;
                    number += nAtBuffer - '0';
                    nAtBuffer = buffer[idxBuffer++];
                }
                if(negative) number *= -1;
                *((int *)va_arg(arg_param, int *)) = number;
                argsRead++;
                if(buffer[idxBuffer] == 0 && fmt[idxFmt + 2] == 0) return argsRead;
                idxFmt += 2;
                idxBuffer--;
                break;
            case 's':
                auxPointer = va_arg(arg_param, char *);
                idxAuxPointer = 0;
                char cAtBuffer = buffer[idxBuffer++];
                while (cAtBuffer != ' ') 
                {
                    if(cAtBuffer == 0 && fmt[idxFmt + 2] == 0){
                        auxPointer[idxAuxPointer] = cAtBuffer;
                        return argsRead + 1;
                    } 
                    if(cAtBuffer == fmt[idxFmt + 2]){ 
                        break;
                    }
                    auxPointer[idxAuxPointer++] = cAtBuffer;
                    cAtBuffer = buffer[idxBuffer++];
                }
                auxPointer[idxAuxPointer] = '\0';
                idxFmt += 2;
                argsRead++;
                idxBuffer--;
                break;
            case 'f':
                negative = 0;
                idxFloat = 0;
                floatNumber = 0;
                decimal = 0;
                char fAtBuffer = buffer[idxBuffer++];
                while((fAtBuffer >= '0' && fAtBuffer <= '9') || fAtBuffer == '-' || fAtBuffer == '.'){
                    if(idxFloat == 0){
                        idxFloat++;
                        if(fAtBuffer == '-'){
                            negative = 1;
                            fAtBuffer = buffer[idxBuffer++];
                            continue;
                        }
                        else
                            negative = 0;                   
                    }
                    if(fAtBuffer == '.'){
                        decimal = 1;
                        fAtBuffer = buffer[idxBuffer++];
                        continue;
                    }
                    else if(fAtBuffer == '-' && idxFloat > 0){
                        printf("\nERROR: Metiste un -\n");
                        return -1;
                    }
                    if(!decimal){
                        floatNumber *= 10;
                        floatNumber += fAtBuffer -'0';
                    }
                    else{
                        decimal *= 10;
                        floatNumber += ((double) (fAtBuffer - '0')) / ((double) decimal);
                    }
                    fAtBuffer = buffer[idxBuffer++];
                }
                if(negative) floatNumber *= -1;
                doublePointer = va_arg(arg_param, double *);
                *doublePointer = floatNumber;
                argsRead++;
                if(buffer[idxBuffer] == 0 && fmt[idxFmt + 2] == 0) return argsRead;
                idxFmt += 2;
                idxBuffer--;
                break;
            case '%':
                if(buffer[idxBuffer + 1] == '%'){
                    idxBuffer += 2;
                    idxFmt += 2;
                }
                break;
            default:
                printf("\nERROR: Formato desconocido\n");
                return -1;
                break;
            }
        }
    }
    return argsRead;
}

int doubleToString(double num, char *buffer)
{
    int neg = 0;
    char *p = buffer, *p1, *p2;
    int digits = 0;
    if (num < 0)
    {
        neg = 1;
        num = -num;
        *p++ = '-';
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
    p1 = buffer + neg;
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
    printf("Familia del procesador: %d\n", processorFamily());
    printf("Modelo de procesador: %d\n", processorModel());
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

int getScreen()
{
    return sys_GetScreen();
}

int getchar()
{
    char buffer[5] = {0};
    sysread(0, buffer, 1);
    char c = buffer[0];
    return (int)c;
}

void putchar(char c)
{
    char buff[2];
    buff[0] = c;
    buff[1] = 0;
    syswrite(1, buff, 1);
}

void temperature(){

}

void printRtc(){
    printf("%d:%d:%d\n", (sysrtc(4)+21)%24, sysrtc(2), sysrtc(0));
}
