#include "standardlib.h"
void loop()
{
    int pid =  getPID();
    while (1)
    {
        printf("loop %d ",pid);
        for (int i = 0; i < 100000000*2; i++)
            ;
    }
}