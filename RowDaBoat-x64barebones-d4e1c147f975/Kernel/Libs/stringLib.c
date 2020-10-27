// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
        des[i] = source[i];
        i++;
    }
    des[i] = '\0';
    return i;
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