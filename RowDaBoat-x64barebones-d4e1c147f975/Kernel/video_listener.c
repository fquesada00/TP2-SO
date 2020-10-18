#include "fileDescriptor.h"
#include "video_driver.h"
#include "scheduler.h"
extern int syscall_read(int fd, char *buffer, int n);
extern file_t *stdout;
extern void blockCurrent(int fd,BlockReason reason);
extern _sti();
extern _cli();
void video_listener()
{
    _cli();
    char buff[BUF_SIZE + 1];
    int n;
    int aux;
    while (1)
    {
        aux = *(stdout->idxWrite) - *(stdout->idxRead);
        if (aux != 0)
        {
            n = syscall_read(1, buff,aux );
            buff[n] = '\0';
            puts(buff);
            //buff[0] = '\0';
        }else{
            blockCurrent(stdout->id,FD_READ);
        }
    }
}
