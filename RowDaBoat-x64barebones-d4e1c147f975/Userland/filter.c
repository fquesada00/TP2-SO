#include "standardlib.h"
#define EOF -1
extern syswrite(int fd, char * buff, int size);
int filter(int argc, char **argv)
{
    int idx = 0;
    char c;
    char writeB[256] = {0};
    while((c = getchar()) != EOF){
        if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u'){
            writeB[0] = c;
            syswrite(1,writeB,1);
        }
    }
    _exit(0);
}