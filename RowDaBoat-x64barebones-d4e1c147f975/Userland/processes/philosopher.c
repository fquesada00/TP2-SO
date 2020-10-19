
// #include "standardlib.h"
// #define ME_RASCO_EL_BOLERO 2
// #define ME_PICA_EL_BAGRE 1
// #define ME_COMO_LA_VIDA 0
// #define CANT_FILOSOFOS_MAX 20
// #define IZQUIERDA ((fil + num - 1) % num)
// #define DERECHA ((fil + 1) % num)

// extern int table[CANT_FILOSOFOS_MAX];
// extern int num;   //var global que indica las posiciones de la tabla de cada filosofo

// extern char * S[CANT_FILOSOFOS_MAX];
// extern void yield();
// void sleep(){
//     for (int i = 0; i < 100000; i++);
// }
// extern void sem();

// // void filosofo()
// // {   
// //     syscallSemOpen("mutex",1,0);
// //     int myNum; //mi indice
// //     syscallSemWait("mutex");
// //     int fil = num++;
// //     syscallSemPost("mutex");
// //     syscallSemOpen(S[fil],0,0);
 
// //     while (1)
// //     {
// //         sleep();
// //         hungry(fil);
// //         sleep();
// //         finishEating(fil);

// //         if (table[IZQUIERDA] == ME_RASCO_EL_BOLERO && table[DERECHA] == ME_RASCO_EL_BOLERO)
// //         {
// //            // printf("Why are we still here? Just to suffer?\n");
// //         }
// //     }
// //     syscallSemClose("mutex");
// //     syscallSemClose(S[fil]);
// // }
// void filosofo()
// {
//     syscallSemOpen("mutex",1,0);
//     syscallSemWait("mutex");
//     int myNum = num++;
//     syscallSemPost("mutex");
//     syscallSemOpen(S[myNum],1,0);
//     int fil = myNum;
//     while (1)
//     {
//         sleep();
//         hungry(myNum);
//         sleep();
//         finishEating(myNum);
//         if (table[IZQUIERDA] == ME_RASCO_EL_BOLERO && table[DERECHA] == ME_RASCO_EL_BOLERO)
//         {
//             //printf("Why are we still here? Just to suffer?\n");
//         }
//     }
// }

// // void hungry(int myNum)
// // {
// //     syscallSemWait("mutex"); //espero a ser el unico accediendo a la mesa
// //     //syscallSemWait("paso");
// //     table[myNum] = ME_PICA_EL_BAGRE;
// //     tryToEat(myNum);
// //     syscallSemPost("mutex");
// //     syscallSemWait(S[myNum]);
// // }
// void hungry(int myNum)
// {
//     syscallSemWait("mutex"); //espero a ser el unico accediendo a la mesa
//     table[myNum] = ME_PICA_EL_BAGRE;
//     tryToEat(myNum);
//     syscallSemPost("mutex");
//     syscallSemWait(S[myNum]);
//     sleep();
// }
// // void tryToEat(int fil)
// // {
// //     if (table[fil] == ME_PICA_EL_BAGRE && table[IZQUIERDA] != ME_COMO_LA_VIDA && table[DERECHA] != ME_COMO_LA_VIDA)
// //     {
// //         table[fil] = ME_COMO_LA_VIDA;
// //         sleep();
// //         //printTableState();
// //         syscallSemPost(S[fil]);

// //     }
// // }
// void tryToEat(int fil)
// {
//     if (table[fil] == ME_PICA_EL_BAGRE && table[IZQUIERDA] != ME_COMO_LA_VIDA && table[DERECHA] != ME_COMO_LA_VIDA)
//     {
//         table[fil] = ME_COMO_LA_VIDA;
//         //printTableState();
//         syscallSemPost(S[fil]);
//     }
    
// }

// // void finishEating(int myNum)
// // {
// //     syscallSemWait("mutex");
// //     table[myNum] = ME_RASCO_EL_BOLERO;
// //     int fil = myNum;
// //     tryToEat(IZQUIERDA);
// //     tryToEat(DERECHA);

// //     syscallSemPost("mutex");
// // }
// void finishEating(int myNum)
// {
//     syscallSemWait("mutex");
//     table[myNum] = ME_RASCO_EL_BOLERO;
//     int fil = myNum;
//     tryToEat(IZQUIERDA);
//     tryToEat(DERECHA);

//     syscallSemPost("mutex");
// }

// // void printTableState()
// // {
// //     for (int i = 0; i < num; i++)
// //     {
// //         printf("%s ",table[i]==2?"E":".");
// //     }
// //     printf("\n\n"); //esto esta fea la impresion pero no se que pasa si me interrumpen, o despues lo cambio de ultima

// // }
// void printTableState()
// {
//     int ok = 0;
//     //printf("Hay %d filosofos\n",num);
//     for (int i = 0; i < num; i++)
//     {
//         printf("%s ", table[i] == ME_COMO_LA_VIDA ? "E" : table[i] == ME_PICA_EL_BAGRE? "W":".");
//         if (table[i] == 2 && table[(i == (num - 1) ? 0 : (i + 1))] == 2)
//         {
//             ok = 1;
//         }
//     }
//     printf("\n\n");
//     // /printf("\tSALIO TODO: %s\n\n\n", ok ? "MAL" : "BIEN"); //esto esta fea la impresion pero no se que pasa si me interrumpen, o despues lo cambio de ultima
//     //syscallSemPrint();
//     //ps();
// }