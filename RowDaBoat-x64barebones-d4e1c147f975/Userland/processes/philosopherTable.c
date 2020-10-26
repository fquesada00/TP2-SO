#include "semLib.h"
#include "processLib.h"
#include "stdioLib.h"
#define ME_RASCO_EL_BOLERO 0
#define ME_PICA_EL_BAGRE 1
#define ME_COMO_LA_VIDA 2
#define ME_VOY_DE_LA_MESA 3
#define CANT_FILOSOFOS_MAX 7
#define IZQUIERDA ((fil + num - 1) % num)
#define DERECHA ((fil + 1) % num)
#define INITIAL 5
extern void filosofo(int argc, char *argv[]);
int table[CANT_FILOSOFOS_MAX] = {3};
int num = 0; //var global que indica las posiciones de la tabla de cada filosofo
int remover = 0;
char *S[CANT_FILOSOFOS_MAX] = {"Garberoglio", "HM", "Godio", "Ortiz", "Mogni", "Socrates", "Platon"};
//"Crito","Heraclito","Nietzsche","Jung","Hegel","Marx","Aristotle","Confucius","Descartes",
// "Godel","Kant","Machiavelli","Lao Tzu"};
int filPID[CANT_FILOSOFOS_MAX];

extern void printTableState();

void philosopherTable(int argc, char *argv[])
{
    semOpen("mutex", 1, 0);
    //sem_open("&mutex", 1,0); //ni idea como va

    for (int i = 0; i < CANT_FILOSOFOS_MAX; i++)
    {
        semOpen(S[i], 0, 0);
    }
    //sem_open(&s[i], 0, 0);
    char *argv2[] = {"filosofo", NULL};
    for (int i = 0; i < CANT_FILOSOFOS_MAX; i++)
    {
        printf("antes");
        filPID[i] = _execv(filosofo, 1, argv2, 0);
        printf("dsp\n");
    }
    while (1)
    {
        char a = getchar();
        if (a == 'a')
        {
            if (num < CANT_FILOSOFOS_MAX)
            {
                semWait("mutex");
                int aux = num;
                int flag = 0;
                for (int i = 0; i < CANT_FILOSOFOS_MAX && !flag; i++)
                {
                    if (table[i] == ME_VOY_DE_LA_MESA)
                    {
                        semPost(S[i]);
                        flag = 1;
                    }
                }
                flag = 0;
                semPost("mutex");
            }
            else
                printf("Creo que si traes a alguien mas que tiene una crisis existencial nos suicidamos los 20");
        }
        else if (a == 'r')
        {
            if (num > INITIAL)
            {
                semWait("mutex");
                remover++;
                semPost("mutex");
            }
            else
                printf("Porfavor no me dejes solo");
        }
        else
        {
            printTableState();
        }
    }
}