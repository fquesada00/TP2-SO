#include <stdint.h>
#include "../include/list.h"
#include "../include/fds.h"

FILE_DESCRIPTOR * fds[TOTAL_FDS] = {0};

int pipe(int fds[2]){
    fds[0] = 0;
    fds[1] = 0;
    for(int i = 0 ; i < TOTAL_FDS ; i++){
        if(!occupiedFds[i]){
            if(!fds[0]){
                fds[0] = i;
                occupiedFds[i] = 1;
                continue;
            }
            fds[1] = i;
            occupiedFds[i] = 1;
            return 1;
        }
    }
    return -1;
}

int close(int fd){
    for(int i = 0 ; i < TOTAL_FDS ; i++){
        if(fd == i && occupiedFds[i]==1){//deberiamos hacer el chequeo q este ocupado o si no lo esta de ultima lo sobreescribe?
            occupiedFds[i] = 0;
            return 1;
        }
    }
    return -1;
}