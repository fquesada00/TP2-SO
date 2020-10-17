#include "include/standardlib.h"
extern int sysread(int fd, char *buff, int bytes);
int leo(){
    char buff[256]={0};
    sysread(0,buff,15);
    printf("%s\n",buff);
    _exit(0);
}