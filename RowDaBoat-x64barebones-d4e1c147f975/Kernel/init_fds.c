#include "fileDescriptor.h"
#include "memory_manager.h"
#include "pipe.h"
extern file_t * stdin;
extern file_t * stdout;
void init_fds(){
    stdin = pMalloc(sizeof(file_t));
    stdin->read = pMalloc(sizeof(char)*BUF_SIZE);
    stdin->write = stdin->read;
    stdin->idxR = 0;
    stdin->idxW = 0;
    stdin->id = 0;
    stdin->type = STDINOUT;
    stdout = pMalloc(sizeof(file_t));
    stdout->read = pMalloc(sizeof(char)*BUF_SIZE);
    stdout->write = stdout->read;
    stdout->idxR = 0;
    stdout->idxW = 0;
    stdout->id = 1;
    stdout->type = STDINOUT;
}