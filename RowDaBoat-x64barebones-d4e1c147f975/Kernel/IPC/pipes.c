#include <stdint.h>
#include "../include/list.h"
#include "../include/fds.h"
#define MAX_PIPES 64
#define BUFFER_SIZE 512

typedef struct PIPE{
    char buffer[BUFFER_SIZE];
    size_t reading[PROCESSES+1];
    size_t writing[PROCESSES+1];
    int writeIndex = 0;
    int readIndex = 0;
}pipe_struct;

pipe_struct pipeArray[MAX_PIPES] = {0};
int pipeIndex = 2;

int pipe(void * entryPoint,int argc,  char*argv[],uint64_t rsp,int read_pipe, int write_pipe){
	init_process_with_pipes(entryPoint,argc,argv,rsp,read_pipe,write_pipe);
	//tendriamos que modificar el PCB o algo donde guardar data del proceso y
	// hacer que guarde un int read_pipe y int write_pipe, donde por defualt son 0 y 1, stdin y stdout.
	//y aca, no tengo idea como ejecuto el proceso, o mentira, returneo y listo, total con el init ya quedan agregados a la lista
	//igual creo que es porque no entiendo fds porque viendo fdslib la implementacion es muy parecida... lpmm


    //creamos PCB con data que nos dan
    //pipe(struct datos1, struct datos2, int fdLectura) -> inicializo todo desde kernel
    //PCB ->  *pipe_struct lectura, *pipe_struct escritura
    //pipe(void *entryPoint, int newFd, int oldFd)
    return -1;
}

int openPipe(){
	return pipeArray++; //estuve pensando, con esta implementacion de buffer, stdin siendo 0 y stdout 1, no tiene sentido
	//tipo tendrian que ser los dos cero! pero nada tengo q pensar
}

int closePipe() {
	//uh esta me hizo poronga
}

void read(int pid,char * buffer) {
	//busco en mi lista de procesos el proceso
	//accedo a el proceso y me fijo el nro que tiene guardaro
	int pipeIndex = searchReadPipeIndex(int pid);
	while(pipeArray[pipeIndex].readIndex < pipeArray[pipeIndex].writeIndex){
		*buffer = pipeArray[pipeIndex].buffer[pipeArray[pipeIndex].readIndex++];
		//me falta hacerlo circular y mañana hago un par de aux para hacerlo leible
	}
	return;
}

void write(int pid,char * buffer) {
	int pipeIndex = searchWritePipeIndex(int pid);
	//lo mismo que arriba y hago un par de aux me voy a mimir
}

//O OTRA OPCION, 
void read(int pipeIndex,char * buffer) {
	strcpy(buffer,pipeArray[pipeIndex]); //obviamente esto no funciona pero es la logica
}

//idem para void write(int pid,char * buffer) OOOO  void write(int pipeIndex, char * buffer)