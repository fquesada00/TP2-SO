#include "include/standardlib.h"
extern syswrite(int fd, char * buff, int size);
int escribo(){
    syswrite(1,"hola como estas",15);
    _exit(0);
}