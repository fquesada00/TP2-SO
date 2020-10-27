// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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