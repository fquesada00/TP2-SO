#include <stdint.h>
#include <stdarg.h>
#include "standardlib.h"

// asumimos fd=1 STDOUT
extern int syswrite(int fd, const char *buff, int bytes);
// asumimos fd=0 STDIN
extern int sysread(int fd, char *buff, int bytes);
extern void inforeg(uint64_t * regs);
extern char *processorName(char*);
extern char *processorExtendedName(char *);
extern int processorModel(void);
extern int processorFamily(void);
extern int divExc();
extern int loadPrgrm(void(*programa)(void));
extern int processorTemperature();
extern unsigned char sysrtc(int);
extern void invalidOpCode();
extern int read_mem(uint64_t address, char * buff);
extern int sys_execv(void *entry_point, int argc, char *argv[]);
extern syscallMalloc(size_t size);
extern syscallFree(void * block);
extern syscallKill(int pid);
extern void syscallProcesses();
extern int syscallBlock(int pid, int block);
extern int getPID();
extern void nice(int pid, int p);
extern int syscallSemOpen(const char *name, size_t value, char created);
extern int syscallSemWait(const char *name);
extern int syscallSemPost(const char *name);
extern int syscallSemClose(const char *name);
extern int syscallPipeClose(int fd, const char *name);
extern int syscallInitProcessWithPipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode);
extern int syscallPipeOpen(int fd[2], const char * name);

void puts(char * c){
    int i = 0;
    while(c[i]){
        putchar(c[i++]);
    }
    putchar('\n');
}

int printf(const char * fmt, ...){
    va_list arg_param;
    va_start(arg_param, fmt);
    int idxFmt = 0, idxBuffer = 0;
    char buffer[256] = {0};
    char * sAtFmt;
    int nAtFmt, nAtFmtLenght, idxsAtFmt, fAtFmtLenght;
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
            //putchar(c);
        }
        else if (idxBuffer > 0)
        {
            idxBuffer--;
            //putchar(c);
        }
    }
    buffer[idxBuffer] = 0;
    if (idxBuffer == 0)
        return argsRead;
    idxBuffer = 0;
    while (fmt[idxFmt])
    {
        if(buffer[idxBuffer] == 0) return argsRead;
        if (fmt[idxFmt] != '%')
        {
            if (buffer[idxBuffer] != fmt[idxFmt])
            {
                return argsRead;
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
                        return argsRead;
                    }
                    number *= 10;
                    number += nAtBuffer - '0';
                    nAtBuffer = buffer[idxBuffer++];
                }
                if(idxNumber == 0) return argsRead;
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
                    if(cAtBuffer == 0){
                        auxPointer[idxAuxPointer] = cAtBuffer;
                        return argsRead + 1;
                    }
                    else if(cAtBuffer == fmt[idxFmt + 2]){ 
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
                        return argsRead;
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
                if(idxFloat == 0) return argsRead;
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
    putchar('\n');
    processorName(buffer);
    printf("Marca del procesador: %s\n", buffer);
    processorExtendedName(buffer);
    printf("Marca del procesador extendida: %s\n", buffer);
    printf("Familia del procesador: %d\n", processorFamily());
    printf("Modelo de procesador: %d\n", processorModel());
}

void printMemoryFromAddress(uint64_t address)
{
    char buff[255]={0};
    if(address < 0){
        printf("\nERROR: %d is not a valid address\n", address);
        return;
    }
    unsigned char *p = (unsigned char *)address;
    read_mem(address,buff);
    for (int i = 0; i < 32; i++)
    {
        printf("%d = %d\n", p, buff[i]);
        p++;
    }
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

void printReg()
{
    uint64_t buffer[20];
    inforeg(buffer);
    char string[128]={0};
    for (int i = 0; i < 20; i++)
    {
        switch (i)
        {
        case 19:
            printf("ss = ");
            break;
        case 18:
            printf("rsp = ");
            break;
        case 17:
            printf("rflags = ");
            break;
        case 16:
            printf("cs = ");
            break;
        case 15:
            printf("rip = ");
            break;
        case 14:
            printf("rax = ");
            break;
        case 13:
            printf("rbx = ");
            break;
        case 12:
            printf("rcx = ");
            break;
        case 11:
            printf("rdx = ");
            break;
        case 10:
            printf("rbp = ");
            break;
        case 9:
            printf("rdi = ");
            break;
        case 8:
            printf("rsi = ");
            break;
        case 7:
            printf("r8 = ");
            break;
        case 6:
            printf("r9 = ");
            break;
        case 5:
            printf("r10 = ");
            break;
        case 4:
            printf("r11 = ");
            break;
        case 3:
            printf("r12 = ");
            break;
        case 2:
            printf("r13 = ");
            break;
        case 1:
            printf("r14 = ");
            break;
        case 0:
            printf("r15 = ");
            break;
        default:
            break;
        }
        uintToBase(buffer[i],string,16);
        printf(string);
        putchar('\n');
    }
    
}

void DivZero()
{
    divExc();
}

int programLoader(void(*program)(void))
{
    return loadPrgrm(program);
}

void invOpCode()
{
    invalidOpCode();
}
void printCoreTemp()
{
    printf("\nCore temperature: %d C\n",processorTemperature());
}


void printRtc(){
    printf("\n%d:%d:%d\n", (sysrtc(4)+21)%24, sysrtc(2), sysrtc(0));
}
int execv(void *entry_point, int argc, char *argv[])
{
    return sys_execv(entry_point,argc,argv);
}

void manShell(){
    printf("\n\n\t\t\t\t\t\t\t\t\t\tSHELL COMMANDS LIST\n\n\n");
    printf("\tinforeg: MUESTRA POR SALIDA ESTANDARD EL VALOR DE LOS\n\tREGISTROS\n\n");
    printf("\t\t");
    printf("Presionar CTRL + R para guardar los registros en el momento\n\tdeseado y luego ejecutar el comando\n\n");
    printf("\tprintmem: RECIBE UNA DIRECCION DE MEMORIA Y REALIZA UN VOLCADO\tDE MEMORIA DE 32 BYTES A PARTIR DE LA DIRECCION INDICADA\n\n");
    printf("\ttime: MUESTRA POR SALIDA ESTANDARD LA HORA DEL SISTEMA\n\n");
    printf("\tprocessor: MUESTRA POR SALIDA ESTANDARD LA MARCA, MARCA\n\tEXTENDIDA, FAMILIA Y MODELO DEL PROCESADOR\n\n");
    printf("\ttemp: MUESTRA POR SALIDA ESTANDARD LA TEMPERATURA DEL\n\tPROCESADOR MEDIDA EN GRADOS CENTIGRADOS\n\n");
    printf("\ttestDivZero: EJECUTA LA EXCEPCION 0 DE TIPO DIVISION POR CERO\n\n");
    printf("\t\tMuestra el funcionamiento de la rutina de atencion \n\tde dicha excepcion\n\n");
    printf("\ttestInvOpCode: EJECUTA LA EXCEPCION 6 DE TIPO INVALID OP CODE\n\n");
    printf("\t\tMuestra el funcionamiento de la rutina \n\tde atencion de dicha excepcion\n\n");
    printf("\thelp: MUESTRA POR SALIDA ESTANDAR ESTE PANEL\n\n");
    printf("\n\n\n");
    printf("\tPARA EJECUTAR CUALQUIER COMANDO, SE DEBE PRESIONAR LA TECLA\n\tENTER AL FINAL\n");
    printf("\n\n\n");
    printf("\tALUMNOS: Quesada, Francisco y Serpe, Octavio\n");
}
void * pMalloc(size_t size){
    return syscallMalloc(size);
}
void pFree(void * block){
    syscallFree(block);
}
void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}
int kill(int pid){
    syscallKill(pid);
}
void ps()
{
    syscallProcesses();
}
int block(int pid,int block)
{
    if (block >1 || block < 0)
        return -1;
    return syscallBlock(pid,block);
}