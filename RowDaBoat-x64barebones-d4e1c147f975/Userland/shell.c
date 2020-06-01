#include "standardlib.h"

void shell()
{
    char command[256]={0};
    int i = 0;
    char c = 0;
    while (strcmp(command,"quit") != 0)
    {
        command[0]=0;
        printf("\nUser:> ");
        i=0;
        while ((c=getchar()) != '\n')
        {
            if(c != '\b'){
                command[i++] = c;
                putchar(c);
            }else if(i > 0){
                i--;
                putchar(c);
            }
        }
        printf("fuera");
        putchar('\n');
        command[i]=0;
        if(strcmp(command,"processor") == 0)
            processorInfo();
        else if(strcmp(command,"printmem"))
        {
            printMemoryFromAddress(0);
        }
    }
    
}