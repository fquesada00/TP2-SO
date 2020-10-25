#include "stdioLib.h"
#include "test_util.h"
#include <stddef.h>
#include "shell.h"
#include "processLib.h"
//TO BE INCLUDED
void endless_loop(){
  while(1);
}

#define MAX_PROCESSES 6 //Should be around 80% of the the processes handled by the kernel

enum State {ERROR, RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

void test_processes(int argc, char * argv[]){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  printf("Starting process test\n");
  int i = 0;
  while (1){
    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
        char * argv[] = {"endless_loop",NULL};
      p_rqs[rq].pid = _execv(endless_loop,1,argv,0);  // TODO: Port this call as required
      printf("execv of process %d return\n",p_rqs[rq].pid);
      if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
        printf("Error creating process\n");               // TODO: Port this as required
        _exit();;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }
  //printf("%d\n",i++);
    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (kill(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                printf("Error killing process\n");        // TODO: Port this as required
                _exit();
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(block(p_rqs[rq].pid,1) == -1){          // TODO: Port this as required
                printf("Error blocking process\n");       // TODO: Port this as required
                _exit();
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
          if(block(p_rqs[rq].pid,0) == -1){            // TODO: Port this as required
            printf("Error unblocking process\n");         // TODO: Port this as required
            _exit();
          }
          p_rqs[rq].state = RUNNING; 
        }
    } 
  }
  _exit();
}
