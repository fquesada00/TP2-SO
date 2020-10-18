#include "fileDescriptor.h"
#include "memory_manager.h"
#include "pipe.h"
extern file_t * stdin;
extern file_t * stdout;
int idFds = 0;
void init_fds(){
    stdin = pMalloc(sizeof(file_t));
    stdin->read = pMalloc(sizeof(char)*BUF_SIZE);
    stdin->write = stdin->read;
    stdin->idxRead = pMalloc(sizeof(int));
    stdin->idxWrite = pMalloc(sizeof(int));
    *stdin->idxWrite = 0;
    *stdin->idxRead = 0;
    stdin->id = idFds++;
    stdin->type = STDINOUT;
    stdout = pMalloc(sizeof(file_t));
    stdout->read = pMalloc(sizeof(char)*BUF_SIZE);
    stdout->write = stdout->read;
    stdout->idxRead = pMalloc(sizeof(int));
    stdout->idxWrite = pMalloc(sizeof(int));
    *stdout->idxRead = 0;
    *stdout->idxWrite = 0;
    stdout->id = idFds++;
    stdout->type = STDINOUT;
}