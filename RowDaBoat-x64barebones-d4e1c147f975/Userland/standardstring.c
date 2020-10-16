#include "standardstring.h"
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

int strcpy(char * des, const char * source){
    int i = 0;
    while(source[i] != 0)
    {
        des[i] = source[i++];
    }
    des[i] = '\0';
}
