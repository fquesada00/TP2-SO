#include "stdioLib.h"
#include "sysLib.h"
#include "processLib.h"
void leo(){
    char buff[256]={0};
    sysread(0,buff,15);
    printf("%s\n",buff);
    _exit();
}