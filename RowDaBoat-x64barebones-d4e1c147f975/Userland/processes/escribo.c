#include "sysLib.h"
#include "processLib.h"
int escribo(){
    syswrite(1,"hola como estas",15);
    _exit();
}