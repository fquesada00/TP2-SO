#include "../include/fds.h"
#include "../include/list.h"

FILE_DESCRIPTOR * fds[TOTAL_FDS] = {0};


void insertAndBlockPid(int fd){
    size_t * buffer = fds[fd]->blockedPids, pid = blockedHeader.current->data.PID;
    int i, done = 0;
    for(i = 0 ; !done && i < TOTAL_FDS ; i++){
        if(buffer[i] == -1){
            buffer[i] = pid;
            blockProcess(pid, fd);
        }
    }
}

void removeAndUnblockPid(int fd){
    size_t * buffer = fds[fd]->blockedPids;
    int i, done = 0;
    for(i = 0 ; !done && i < TOTAL_FDS ; i++){
        if(buffer[i] != -1) done = 1;
    }
    listElem_t *list = blockedHeader.current;
    while(list!=NULL){
        if(buffer[i] == list->data.PID){
            readyProcess(buffer[i]);
            buffer[i] = -1;
        }
    }
}

int close(int fd){

}

void add(int fd){

}

void retrieveFds(size_t pid, int * read, int * write){
    
}