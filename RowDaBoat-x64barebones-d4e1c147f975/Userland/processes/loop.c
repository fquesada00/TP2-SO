#include "stdioLib.h"
#include "processLib.h"
void loop()
{
    int pid =  _getPID();
    while (1)
    {
        printf("loop %d ",pid);
        for (int i = 0; i < 100000000*2; i++)
            ;
    }
}