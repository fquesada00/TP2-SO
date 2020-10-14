#include "fileDescriptor.h"
#include "video_driver.h"
extern int syscall_read(int fd, char *buffer, int n);

void video_listener(){
    char buff[BUF_SIZE+1];
    while(1){
        syscall_read(0,buff,BUF_SIZE);
        puts(buff);
    }
}
