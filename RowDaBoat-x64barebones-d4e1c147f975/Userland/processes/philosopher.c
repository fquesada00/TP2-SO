#include "semLib.h"
#include "processLib.h"
#include "stdioLib.h"
#define ME_RASCO_EL_BOLERO 0
#define ME_PICA_EL_BAGRE 1
#define ME_COMO_LA_VIDA 2
#define ME_VOY_DE_LA_MESA 3

#define CANT_FILOSOFOS_MAX 20
extern int table[CANT_FILOSOFOS_MAX];
extern int num;   //var global que indica las posiciones de la tabla de cada filosofo
extern int remover;
extern char * S[CANT_FILOSOFOS_MAX];



void hungry(int myNum);
void removerAUnVago();
void printTableState();
int filIzquierda(int myNum);
int filDerecha(int myNum);
void tryToEat(int fil);
void finishEating(int myNum);
// void filosofo()
// {   
//     syscallSemOpen("mutex",1,0);
//     int myNum; //mi indice
//     syscallSemWait("mutex");
//     int fil = num++;
//     syscallSemPost("mutex");
//     syscallSemOpen(S[fil],0,0);
 
//     while (1)
//     {
//         sleep();
//         hungry(fil);
//         sleep();
//         finishEating(fil);

//         if (table[IZQUIERDA] == ME_RASCO_EL_BOLERO && table[DERECHA] == ME_RASCO_EL_BOLERO)
//         {
//            // printf("Why are we still here? Just to suffer?\n");
//         }
//     }
//     syscallSemClose("mutex");
//     syscallSemClose(S[fil]);
// }
void sleep(int j){
    for(int i = 0; i< j*100000000; i++);
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
        if (table[filIzquierda(myNum)] == ME_RASCO_EL_BOLERO && table[filDerecha(myNum)] == ME_RASCO_EL_BOLERO)
        {
        }
    }
}

// void hungry(int myNum)
// {
//     syscallSemWait("mutex"); //espero a ser el unico accediendo a la mesa
//     //syscallSemWait("paso");
//     table[myNum] = ME_PICA_EL_BAGRE;
//     tryToEat(myNum);
//     syscallSemPost("mutex");
//     syscallSemWait(S[myNum]);
// }
void hungry(int myNum)
{
    semWait("mutex"); //espero a ser el unico accediendo a la mesa
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

    //c.c..c. rrr
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
// void tryToEat(int fil)
// {
//     if (table[fil] == ME_PICA_EL_BAGRE && table[IZQUIERDA] != ME_COMO_LA_VIDA && table[DERECHA] != ME_COMO_LA_VIDA)
//     {
//         table[fil] = ME_COMO_LA_VIDA;
//         sleep();
//         //printTableState();
//         syscallSemPost(S[fil]);

//     }
// }
void tryToEat(int fil)
{
    if (table[fil] == ME_PICA_EL_BAGRE && table[filIzquierda(fil)] != ME_COMO_LA_VIDA && table[filDerecha(fil)] != ME_COMO_LA_VIDA)
    {
        printTableState();
        table[fil] = ME_COMO_LA_VIDA;
        // printf("entre %d\n",fil);
        //printTableState();
        semPost(S[fil]);
  
    }
    
}

// void finishEating(int myNum)
// {
//     syscallSemWait("mutex");
//     table[myNum] = ME_RASCO_EL_BOLERO;
//     int fil = myNum;
//     tryToEat(IZQUIERDA);
//     tryToEat(DERECHA);

//     syscallSemPost("mutex");
// }
void finishEating(int myNum)
{
	removerAUnVago();
    semWait("mutex");
    table[myNum] = ME_RASCO_EL_BOLERO;
    tryToEat(filIzquierda(myNum));
    tryToEat(filDerecha(myNum));
    semPost("mutex");
}

// void printTableState()
// {
//     for (int i = 0; i < num; i++)
//     {
//         printf("%s ",table[i]==2?"E":".");
//     }
//     printf("\n\n"); //esto esta fea la impresion pero no se que pasa si me interrumpen, o despues lo cambio de ultima

// }
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

int ok = 0;
void printTableState()
{
    //printf("Hay %d filosofos\n",num);
    for (int i = 0; i < CANT_FILOSOFOS_MAX; i++)
    {
    	if(table[i] != ME_VOY_DE_LA_MESA){
        	printf("%s ", table[i] == ME_COMO_LA_VIDA ? "E" : table[i] == ME_PICA_EL_BAGRE? "W":".");
        	if (table[i] == ME_COMO_LA_VIDA && (table[filIzquierda(i)] == ME_COMO_LA_VIDA || table[filDerecha(i)] == ME_COMO_LA_VIDA))
        	{
        	    ok = 1;
       	 	}
    	}
    }
    printf("\n\n");
    // /printf("\tSALIO TODO: %s\n\n\n", ok ? "MAL" : "BIEN"); //esto esta fea la impresion pero no se que pasa si me interrumpen, o despues lo cambio de ultima
    //syscallSemPrint();
    //ps();
}