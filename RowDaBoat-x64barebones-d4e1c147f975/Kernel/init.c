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
extern int execvAuxi(void * entry,int argc, char * argv[]);
void idle();
void init(){
    _sti();
    char * argv1[] ={"shell",NULL};
    int shell_pid = execvAuxi(sampleCodeModuleAddress,1,argv1);
    char * argv2[] = {"idle",NULL};
    idle_pid = execvAuxi(idle,1,argv2);
    nice(idle_pid,9);
    blockProcess(idle_pid,1);
    exit(0);
}

void idle(){
    while (1)
    {
        //puts("idle ");
        _hlt();   
    }
}