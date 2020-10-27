// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdioLib.h"
#include "sysLib.h"
#include "processLib.h"
#define EOF -1
void filter(int argc, char **argv)
{
    char c;
    char writeB[256] = {0};
    while((c = getchar()) != EOF){
        if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u'){
            writeB[0] = c;
            syswrite(1,writeB,1);
        }
    }
    getchar();
    _exit();
}