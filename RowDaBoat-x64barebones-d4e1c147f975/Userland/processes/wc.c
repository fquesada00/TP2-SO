#include "stdioLib.h"
#include "sysLib.h"
#include "processLib.h"
#include <stddef.h>
#define EOF -1
int wc(){
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