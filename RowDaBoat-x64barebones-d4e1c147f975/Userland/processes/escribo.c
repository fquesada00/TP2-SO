// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "sysLib.h"
#include "processLib.h"
void escribo(){
    syswrite(1,"hola como estas",15);
    _exit();
}