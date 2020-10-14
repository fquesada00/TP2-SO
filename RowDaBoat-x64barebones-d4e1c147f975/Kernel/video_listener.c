#include "fileDescriptor.h"
#include "video_driver.h"
extern int syscall_read(int fd, char *buffer, int n);
extern file_t *stdout;
extern void blockCurrent(int fd);

void video_listener()
{
    char buff[BUF_SIZE + 1];
    int n;
    int aux;
    while (1)
    {
        aux = (stdout->idxW) - (stdout->idxR);
        if (aux != 0)
        {
            n = syscall_read(1, buff,aux );
            buff[n] = '\0';
            puts(buff);
            //buff[0] = '\0';
        }else{
            blockCurrent(1);
        }
    }
}
