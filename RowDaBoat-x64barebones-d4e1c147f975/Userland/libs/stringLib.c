#include "stringLib.h"
/* return 1 if s is greater than v
** 0 if s is equal to v
** -1 if s is lower than v */
int isNum(int a);
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

int strcpy(char * des, const char * source){
    int i = 0;
    while(source[i] != 0)
    {
        des[i] = source[i];
        i++;
    }
    des[i] = '\0';
    return i;
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
    int aux;
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
int myAtoi(char *str)
{
    int res = 0;
    int neg = 0;
    for (int i = 0; str[i] != 0; i++)
    {
        if (str[i] == '-')
        {
            neg = 1;
            continue;
        }else if(isNum(str[i])){
        res = res * 10 + str[i] - '0';
        }
        else return -1;
    }
    if (neg)
        res *= -1;
    return res;
}

int isNum(int a){
    return a == '0' ||
           a == '1' ||
           a == '2' ||
           a == '3' ||
           a == '4' ||
           a == '5' ||
           a == '6' ||
           a == '7' ||
           a == '8' ||
           a == '9';

}
int strcat(char* destination, const char* source)
{
	// make ptr point to the end of destination string
	char* ptr = destination + strlen(destination);

	// Appends characters of source to the destination string
	while (*source != '\0')
		*ptr++ = *source++;

	// null terminate destination string
	*ptr = '\0';
    int i  = strlen(destination);

	// destination is returned by standard strcat()
	return i;
}