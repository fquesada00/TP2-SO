#include <stdint.h>
#include "../include/list.h"
#include "../include/fds.h"
#define MAX_PIPES 64
typedef struct PIPE{
    char buffer[BUFFER_SIZE];
    size_t reading[PROCESSES+1];
    size_t writing[PROCESSES+1];
}pipe_struct;

pipe_struct pipeArray[MAX_PIPES] = {0};

int pipe(){
    //creamos PCB con data que nos dan
    //pipe(struct datos1, struct datos2, int fdLectura) -> inicializo todo desde kernel
    //PCB ->  *pipe_struct lectura, *pipe_struct escritura
    //pipe(void *entryPoint, int newFd, int oldFd)
    return -1;
}
