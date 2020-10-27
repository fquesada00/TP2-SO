// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdioLib.h"
#include "sysLib.h"
#include "processLib.h"
void leo(){
    char buff[256]={0};
    sysread(0,buff,15);
    printf("%s\n",buff);
    _exit();
}