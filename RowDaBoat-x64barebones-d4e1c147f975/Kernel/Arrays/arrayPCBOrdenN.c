// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <stddef.h>
#include "arrayPCBOrdenN.h"
int remove(int dim, PCB *element,PCB *array[]){
    for(int i = 0 ; i < dim ; i++){
        if(array[i] == element){
            array[i] = NULL;
            return 0;
        }
    }
    return 1;
}
int add(int dim, PCB *element, PCB *array[]){
    for(int i = 0 ; i < dim ; i++){
        if(array[i] == NULL){
            array[i] = element;
            return 0;
        }
    }
    return 1;
}
