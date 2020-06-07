#include "standardlib.h"
extern int syswrite(int fd, const char *buff, int bytes);
void shell()
{
    printf("\nInitializing shell\n");
    char command[256]={0};
    int argsRead, memoryAddress, quit = 0;
    do{
        printf("\nUser:> ");
        argsRead = scanf("%s %d",command, &memoryAddress);
        putchar('\n');
        if(argsRead == 1){
            if(strcmp(command,"processor") == 0) processorInfo();
            else if(strcmp(command, "inforeg") == 0) printReg();
            else if(strcmp(command, "testDivZero") == 0) DivZero();
            else if(strcmp(command, "testInvOpCode") == 0) invOpCode();
            else if(strcmp(command, "time") == 0) printRtc();
            else if(strcmp(command, "help") == 0) manShell();
            else if(strcmp(command, "quit") == 0) quit = 1;
            else if(strcmp(command,"temp") == 0) printCoreTemp();
            else printf("\n'%s' is not a valid command\n",command);
        }
        else if(argsRead == 2){
            if(strcmp(command,"printmem") == 0) printMemoryFromAddress(memoryAddress);
            else printf("\n'%s' is not a valid command\n",command);
        }
    }while(!quit);
}