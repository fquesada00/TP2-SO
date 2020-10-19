#include "idle.h"
#include "video_listener.h"
#include "interrupts.h"
#include "scheduler.h"
#include "init.h"
#include "processLib.h"
#include <stdint.h>
extern void *const sampleCodeModuleAddress;
extern int idle_pid;
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
    exit();
}
