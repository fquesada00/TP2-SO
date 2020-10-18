#include "standardlib.h"
#define EOF -1
void cat(){
    char c;
    while((c = getchar()) != EOF)
    {
        putchar(c);
    } 
    _exit(0);
}