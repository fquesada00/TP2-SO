#include "stdioLib.h"
#include <stdarg.h>
#include "stringLib.h"
#include "sysLib.h"
void puts(char *c)
{
    int i = 0;
    while (c[i])
    {
        putchar(c[i++]);
    }
    putchar('\n');
}

int printf(const char *fmt, ...)
{
    va_list arg_param;
    va_start(arg_param, fmt);
    int idxFmt = 0, idxBuffer = 0;
    char buffer[256] = {0};
    char *sAtFmt;
    int nAtFmt, nAtFmtLenght, idxsAtFmt, fAtFmtLenght;
    double fAtFmt;
    while (fmt[idxFmt])
    {
        if (fmt[idxFmt] != '%')
        {
            buffer[idxBuffer++] = fmt[idxFmt++];
        }
        else
        {
            switch (fmt[idxFmt + 1])
            {
            case 'c':
                buffer[idxBuffer++] = (char)va_arg(arg_param, int);
                idxFmt += 2;
                break;
            case 'd':
                nAtFmt = va_arg(arg_param, int);
                if (nAtFmt < 0)
                {
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
                while (sAtFmt[idxsAtFmt])
                {
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
    if (idxBuffer > 0)
    {
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
    char *auxPointer;
    double *doublePointer;
    while ((c = getchar()) != '\n')
    {
        if (c != '\b')
        {
            buffer[idxBuffer++] = (char)c;
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
        if (buffer[idxBuffer] == 0)
            return argsRead;
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
                if (idxNumber == 0)
                    return argsRead;
                if (negative)
                    number *= -1;
                *((int *)va_arg(arg_param, int *)) = number;
                argsRead++;
                if (buffer[idxBuffer] == 0 && fmt[idxFmt + 2] == 0)
                    return argsRead;
                idxFmt += 2;
                idxBuffer--;
                break;
            case 's':
                auxPointer = va_arg(arg_param, char *);
                idxAuxPointer = 0;
                char cAtBuffer = buffer[idxBuffer++];
                while (cAtBuffer != ' ')
                {
                    if (cAtBuffer == 0)
                    {
                        auxPointer[idxAuxPointer] = cAtBuffer;
                        return argsRead + 1;
                    }
                    else if (cAtBuffer == fmt[idxFmt + 2])
                    {
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
                while ((fAtBuffer >= '0' && fAtBuffer <= '9') || fAtBuffer == '-' || fAtBuffer == '.')
                {
                    if (idxFloat == 0)
                    {
                        idxFloat++;
                        if (fAtBuffer == '-')
                        {
                            negative = 1;
                            fAtBuffer = buffer[idxBuffer++];
                            continue;
                        }
                        else
                            negative = 0;
                    }
                    if (fAtBuffer == '.')
                    {
                        decimal = 1;
                        fAtBuffer = buffer[idxBuffer++];
                        continue;
                    }
                    else if (fAtBuffer == '-' && idxFloat > 0)
                    {
                        return argsRead;
                    }
                    if (!decimal)
                    {
                        floatNumber *= 10;
                        floatNumber += fAtBuffer - '0';
                    }
                    else
                    {
                        decimal *= 10;
                        floatNumber += ((double)(fAtBuffer - '0')) / ((double)decimal);
                    }
                    fAtBuffer = buffer[idxBuffer++];
                }
                if (idxFloat == 0)
                    return argsRead;
                if (negative)
                    floatNumber *= -1;
                doublePointer = va_arg(arg_param, double *);
                *doublePointer = floatNumber;
                argsRead++;
                if (buffer[idxBuffer] == 0 && fmt[idxFmt + 2] == 0)
                    return argsRead;
                idxFmt += 2;
                idxBuffer--;
                break;
            case '%':
                if (buffer[idxBuffer + 1] == '%')
                {
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
