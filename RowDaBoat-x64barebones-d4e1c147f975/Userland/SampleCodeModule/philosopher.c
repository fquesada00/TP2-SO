
// #define ME_RASCO_EL_BOLERO 0
// #define ME_PICA_EL_BAGRE 1
// #define ME_COMO_LA_VIDA 2
// #define CANT_FILOSOFOS 6
// #define IZQUIERDA ((fil + CANT_FILOSOFOS - 1) % CANT_FILOSOFOS)
// #define DERECHA ((fil + 1) % CANT_FILOSOFOS)

// extern int *table;
// extern int num;   //var global que indica las posiciones de la tabla de cada filosofo
// int myNum; //mi indice

// extern char * S[CANT_FILOSOFOS];

// void filosofo()
// {
//     myNum = num++;
//     int fil = myNum; 
//     while (1)
//     {
//         sleep(1);
//         hungry();
//         sleep(1);
//         finishEating();
//         if (table[IZQUIERDA] == ME_RASCO_EL_BOLERO && table[DERECHA] == ME_RASCO_EL_BOLERO)
//         {
//             printf("Why are we still here? Just to suffer?\n");
//         }
//     }
// }

// void hungry()
// {
//     sem_wait("mutex"); //espero a ser el unico accediendo a la mesa
//     table[myNum] = ME_PICA_EL_BAGRE;
//     tryToEat(myNum);
//     sem_post("mutex");
//     sem_wait(S[myNum]);
//     sleep(1);
// }
// void tryToEat(int fil)
// {
//     if (table[fil] == ME_PICA_EL_BAGRE && table[IZQUIERDA] != ME_COMO_LA_VIDA && table[DERECHA] != ME_COMO_LA_VIDA)
//     {
//         table[fil] = ME_COMO_LA_VIDA;
//         sleep(2);
//         printTableState();
//         sem_post(S[fil]);
//     }
// }

// void finishEating()
// {
//     sem_wait("mutex");
//     table[myNum] = ME_RASCO_EL_BOLERO;
//     int fil = myNum;
//     tryToEat(IZQUIERDA);
//     tryToEat(DERECHA);

//     sem_post("mutex");
// }

// void printTableState()
// {
//     for (int i = 0; i < CANT_FILOSOFOS; i++)
//     {
//         printf("%s %d",S[i],table[i]);
//     }
//     printf("\n"); //esto esta fea la impresion pero no se que pasa si me interrumpen, o despues lo cambio de ultima
// }