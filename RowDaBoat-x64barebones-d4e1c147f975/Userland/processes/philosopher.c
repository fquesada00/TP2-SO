// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "semLib.h"
#include "processLib.h"
#include "stdioLib.h"
#define ME_RASCO_EL_BOLERO 0
#define ME_PICA_EL_BAGRE 1
#define ME_COMO_LA_VIDA 2
#define ME_VOY_DE_LA_MESA 3

#define CANT_FILOSOFOS_MAX 20
extern int table[CANT_FILOSOFOS_MAX];
extern int num;   
extern int remover;
extern char * S[CANT_FILOSOFOS_MAX];



void hungry(int myNum);
void removerAUnVago();
void printTableState();
int filIzquierda(int myNum);
int filDerecha(int myNum);
void tryToEat(int fil);
void finishEating(int myNum);

void sleep(int j){
    for(int i = 0; i< j*10000000; i++);
}
void filosofo(int argc,char * argv[])
{
    semOpen("mutex",1,0);
    semWait("mutex");
    int myNum = num++;
    semPost("mutex");
    semOpen(S[myNum],1,0);
    while (1)
    {
        _yield();
        sleep(1);
        hungry(myNum);
        _yield();
        sleep(2);
        finishEating(myNum);
    }
}


void hungry(int myNum)
{
    semWait("mutex");
    table[myNum] = ME_PICA_EL_BAGRE;
    tryToEat(myNum);
    semPost("mutex");
    semWait(S[myNum]);
}

void removerAUnVago(){
    semWait("mutex");
    if(remover == 0) {
        semPost("mutex");
        return;
    }

    for(int i = 0; i < CANT_FILOSOFOS_MAX; i++) 
        if(table[i] == ME_COMO_LA_VIDA){ 
            table[i] = ME_VOY_DE_LA_MESA;
            remover--;
            num--;
            semPost("mutex");
			semWait(S[i]);
            num++;
            hungry(i);
            return;
        }
    semPost("mutex");
}

void tryToEat(int fil)
{
    if (table[fil] == ME_PICA_EL_BAGRE && table[filIzquierda(fil)] != ME_COMO_LA_VIDA && table[filDerecha(fil)] != ME_COMO_LA_VIDA)
    {
        printTableState();
        table[fil] = ME_COMO_LA_VIDA;
        semPost(S[fil]);
  
    }
    
}

void finishEating(int myNum)
{
	removerAUnVago();
    semWait("mutex");
    table[myNum] = ME_RASCO_EL_BOLERO;
    tryToEat(filIzquierda(myNum));
    tryToEat(filDerecha(myNum));
    semPost("mutex");
}

int filIzquierda(int myNum) {
    for(int i = myNum - 1;i != -2; i-- ) {
        if(i == myNum){
            i = (i-1);
        }
        if(i== -1)
            i = CANT_FILOSOFOS_MAX - 1;
        if(table[i] != ME_VOY_DE_LA_MESA)
            return i;

    }
    return -2;
}
int filDerecha(int myNum) {
    for(int i = myNum  + 1;i <=CANT_FILOSOFOS_MAX+1; i++ ) {
        if(i == myNum){
            i = (i+1);
        }
        if(i== CANT_FILOSOFOS_MAX)
            i = 0;
        if(table[i] != ME_VOY_DE_LA_MESA)
            return i;
    }
    return -2;
}

void printTableState()
{
    for (int i = 0; i < CANT_FILOSOFOS_MAX; i++)
    {
    	if(table[i] != ME_VOY_DE_LA_MESA){
        	printf("%s ", table[i] == ME_COMO_LA_VIDA ? "E" : table[i] == ME_PICA_EL_BAGRE? "W":".");
    	}
    }
    printf("\n\n");
}