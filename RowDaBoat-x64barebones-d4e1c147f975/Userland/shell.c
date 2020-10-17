#include "standardlib.h"
extern int syswrite(int fd, const char *buff, int bytes);
extern test_mm();
extern void test_processes();
extern void loop();
extern void test_prio();
extern void test_sync();
extern void test_no_sync();
extern int philosopherTable();
extern void waitPID(int pid);
void shell()
{
    //test_no_sync();
    //test_sync();
    printf("\nInitializing shell\n");
    printf("\nType 'help' to see the shell commands list\n");
    //test_mm();
    //test_processes();
    //test_prio();
    char * argv[] = {"philosopherTable",NULL};
    //philosopherTable();
    //int p = execv(philosopherTable,1,argv);
    //waitPID(p);
    char command[256]={0};
    int argsRead, memoryAddress,arg3;
    do{
        printf("\nUser:> ");
        argsRead = scanf("%s %d %d",command, &memoryAddress,&arg3);
        putchar('\n');
        if(argsRead == 1){
            if(strcmp(command,"cpuInfo") == 0) processorInfo();
            else if(strcmp(command, "inforeg") == 0) printReg();
            else if(strcmp(command, "testDivZero") == 0) DivZero();
            else if(strcmp(command, "testInvOpCode") == 0) invOpCode();
            else if(strcmp(command, "time") == 0) printRtc();
            //mem() imprime el estado de la memoria 
            //ps() imprime la lista de todos los procesos con: nombre, ID, prioridad, stack y base pointer, foreground y otras cosas
            //loop: imprime su ID con un saludo cada determinados segundos
            //kill(ID): mata un proceso dado su id
            // agregar al help DEBERÁ tener un apartado con el listado de tests provistos por la cátedra.

            //nice(ID, prioridad):cambia la prioridad de un proceso
            //block(ID): cambia el estado entre bloqueado y listo con su ID
            //sem: imprime lista de todos los semaforos con: estado, procesos bloqueados en cada sem, otra variable q quieras
            //cat: imprime el stdin por como lo recibe
            //wc: cuenta la cantidad de lineas del input.
            //filter: filtra las vocales del input
            //pipe: imprime las listas de todos los pipes con sus propiedades: estado, procesos bloq en c/u, y otras cosas q quieras
            
            /*phylo: imprime el problema de los filosofos, se debe poder cambiar la cantidad de filosofos en runtime con "a" agregar, "r" remov.
            Tambien debera mostrar el estado de la mesa E..E.. algo asi,*/


            /* LISTA DE SYSCALLS QUE HAY QUE HACER:
            syscall: processStructure[] processList(); //Termina en NULL para no devolver el size
            syscall: uint64_t getID(); //Devuelve el ID del proceso
            syscall: int kill(uint64_t ID) //Devuelve -1 dependiendo exito o fracaso
            syscall: int nice(uint64_t ID, char prioridad) //Devuelve -1 si fracaso
            syscall: int cockblock(uint64_t ID,char block) //0 si lo quiero bloqueado 1 si lo quiero liberar
            syscall: semaphoreStructure[] semaphoreList(); //Termina en NULL para no devolver el size
            syscall: semaphoreStructure getSemaphoreDate(char[] nombre) //Le paso el nombre del semaphoro y me devuelve la data
            syscall: pipeStructure[] pipeList(); //Termina en NULL para no devolver el size
            syscall: de pipes y semaforos;
            */
            else if(strcmp(command, "help") == 0) manShell();
            else if(strcmp(command,"coreTemp") == 0) printCoreTemp();
            else if(strcmp(command,"printmem")==0) printf("\nprintmem requires an input address\nUsage: 'printmem [ADDRESS]'\n");
            else if (strcmp(command,"calc") == 0)
            {
                char * argv[] = {'calc',NULL};
                printf("sigo aca!\n");
            }
            else if(strcmp(command,"loop") == 0){ 
                char * argvLoop[]={"loop",NULL};
                execv(loop,1,argvLoop); }
            else if(strcmp(command,"ps") == 0){
                ps();
            }
            else if(strcmp(command,"sem") == 0){
                syscallSemPrint();
            }
            else printf("\n'%s' is not a valid command\nType 'help' to see the shell commands list\n",command);
        }
        else if(argsRead == 2){
            if(strcmp(command,"printmem") == 0) printMemoryFromAddress(memoryAddress);
            else if(strcmp(command,"kill") == 0) kill(memoryAddress);
            else printf("\n'%s' is not a valid command\nType 'help' to see the shell commands list\n",command);
        }
        else if(argsRead == 3)
        {
            if(strcmp(command,"block") == 0) block(memoryAddress,arg3);
            else if(strcmp(command,"nice" == 0)) nice(memoryAddress,arg3);
        }
    }while(1);
}