#include <stdint.h>
#include "../include/list.h"
#include "../include/fds.h"


int pipe(int fds[2]){
    fds[0] = 0;
    fds[1] = 0;
    retrieveFds(getpid(),&fds[0],&fds[1]);
    return -1;
}
