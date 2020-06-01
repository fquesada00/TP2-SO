#include "standardlib.h"

void shell()
{
    char command[256];
    int i = 0;
    int c = 0;
    while (strcmp(command,"quit") != 0)
    {
        printf("User:> ");
        i=0;
        while ((c=getchar()) != '/n')
        {
            putchar(c);
            command[i++] = c;
        }
        printf("Sali");
        command[i]=0;
        if(strcmp(command,"processor"))
            processorInfo();
    }
    
}