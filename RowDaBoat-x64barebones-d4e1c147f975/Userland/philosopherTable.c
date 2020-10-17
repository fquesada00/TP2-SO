#include "standardlib.h"
#define ME_RASCO_EL_BOLERO 0
#define ME_PICA_EL_BAGRE 1
#define ME_COMO_LA_VIDA 2
#define CANT_FILOSOFOS_MAX 20
#define IZQUIERDA ((fil + num - 1) % num    )
#define DERECHA ((fil + 1) % num)
#define INITIAL 5
extern void filosofo();
int table[CANT_FILOSOFOS_MAX];
int num = 0; //var global que indica las posiciones de la tabla de cada filosofo

char *S[CANT_FILOSOFOS_MAX] = {"Garberoglio","HM","Godio","Ortiz","Mogni","Socrates","Platon","Crito","Heraclito","Nietzsche","Jung","Hegel","Marx","Aristotle","Confucius","Descartes",
"Godel","Kant","Machiavelli","Lao Tzu"};
int filPID[CANT_FILOSOFOS_MAX];

extern void printTableState();

int philosopherTable()
{
    
    syscallSemOpen("mutex", 1,0); 
    char * argv[] = {"filosofo",NULL};
    int aux;
    for(int i = 0; i< INITIAL;i++) {
        syscallSemOpen(S[i], 0, 0);
    }
    for(int i = 0; i< INITIAL;i++) {
        filPID[i] = execv(filosofo,1,argv); //uso aux porque no se si num se me aumenta
    }

    while(1) {  
        char a = getchar();
        if(a == 'a') {
            if(num < CANT_FILOSOFOS_MAX){
                aux = num;
                syscallSemOpen(S[num], 0, 0);
                filPID[aux] = execv(filosofo,1,argv); //uso aux porque no se si num se me aumenta
            }
            else
                printf("Creo que si traes a alguien mas que tiene una crisis existencial nos suicidamos los 20");
        }
        else if(a == 'r') {
            if(num > INITIAL) {
                syscallSemWait("mutex");
                kill(filPID[--num]);
                table[num] = 0;
                syscallSemClose(S[num]);
                syscallSemPost("mutex");    
            }
            else
                printf("Porfavor no me dejes solo");
        }
        else if(a == '\n'){
            printTableState();

        }            
    }
    for(int i = 0; i< num;i++) {
        syscallSemClose(S[i]);
        syscallSemWait("mutex");
        kill(filPID[i]);
        table[i] = 0;
        syscallSemClose(S[i]);
        syscallSemPost("mutex");
    }
    syscallSemClose("mutex");
    return;
}