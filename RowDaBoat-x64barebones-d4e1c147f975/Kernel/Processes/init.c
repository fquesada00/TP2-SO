// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "idle.h"
#include "video_listener.h"
#include "interrupts.h"
#include "scheduler.h"
#include "init.h"
#include <stdint.h>
extern void *const sampleCodeModuleAddress;
extern int idle_pid;
void init(){
    _cli();
    char * argv2[] = {"idle",NULL};
    idle_pid = execvAuxi(idle,1,argv2,0);
    nice(idle_pid,10);
    blockProcess(idle_pid,1);
    char * argv3[] = {"video_listener",NULL};
    execvAuxi(video_listener,1,argv3,0);
    char * argv1[] ={"shell",NULL};
    execvAuxi(sampleCodeModuleAddress,1,argv1,1);
    pExit();
}
