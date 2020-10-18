#include <stdint.h>
#include "standardlib.h"

#define MINOR_WAIT 10000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      100000000                              // TODO: Long enough to see theese processes beeing run at least twice

void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop2(){
  uint64_t pid = getPID();

  while(1){
    printf("%d ",pid);
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;
  char * argv[] = {"endless_loop",NULL};
  for(i = 0; i < TOTAL_PROCESSES; i++)
  {
    pids[i] = execv(endless_loop2,1,argv,0);
  }

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        printf("setting %d to low\n",pids[i]);
        nice(pids[i], 10); //lowest priority 
        break;
      case 1:
        printf("setting %d to medium\n",pids[i]);
        nice(pids[i], 5); //medium priority
        break;
      case 2:
        printf("setting %d to high\n",pids[i]);
        nice(pids[i], 0); //highest priority
        break;
    }
  }

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i],1);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        printf("setting %d to medium\n",pids[i]);
        nice(pids[i], 5); //medium priority 
        break;
      case 1:
        printf("setting %d to medium\n",pids[i]);
        nice(pids[i], 5); //medium priority
        break;
      case 2:
        printf("setting %d to medium\n",pids[i]);
        nice(pids[i], 5); //medium priority
        break;
    }
  }

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i],0);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);
}