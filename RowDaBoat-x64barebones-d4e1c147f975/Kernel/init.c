#include <stdint.h>
#include <stddef.h>
#include "video_driver.h"
int idle_pid;
extern _hlt();
extern _cli();
extern _sti();
extern void * const sampleCodeModuleAddress;
extern int init_process(void *entry_point, int argc, char *argv[], uint64_t rsp);
extern int nice(int pid, int p);
extern int blockProcess(int pid,int b);
extern int exit(int s);
extern int execvAuxi(void * entry,int argc, char * argv[],int fg);
extern void video_listener();
extern _int20();
void idle();
void init(){
    _cli();
    char * argv2[] = {"idle",NULL};
    idle_pid = execvAuxi(idle,1,argv2,0);
    nice(idle_pid,9);
    blockProcess(idle_pid,1);
    char * argv3[] = {"video_listener",NULL};
    int video_listener_pid = execvAuxi(video_listener,1,argv3,0);
    char * argv1[] ={"shell",NULL};
    int shell_pid = execvAuxi(sampleCodeModuleAddress,1,argv1,1);
    exit(0);
}

void idle(){
    
    while (1)
    {
        //puts("idle ");
        _hlt();  
    }
}