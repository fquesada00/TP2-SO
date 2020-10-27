// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdioLib.h"
#include "sysLib.h"
#include "processLib.h"
#include <stddef.h>
#define EOF -1
void wc(int argc, char * argv[]){
    size_t bytes = 0, lines = 0;
    char c;
    while((c = getchar()) != EOF){
        if(c == '\n'){
            lines++;
        }
        bytes++;
    }
    printf("Lineas: %d\nBytes: %d\n",lines,bytes);
    _exit();
}