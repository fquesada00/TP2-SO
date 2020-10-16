// #define ME_RASCO_EL_BOLERO 0
// #define ME_PICA_EL_BAGRE 1
// #define ME_COMO_LA_VIDA 2
// #define CANT_FILOSOFOS 5
// #define IZQUIERDA ((fil + CANT_FILOSOFOS - 1) % CANT_FILOSOFOS)
// #define DERECHA ((fil + 1) % CANT_FILOSOFOS)

// int table[CANT_FILOSOFOS];
// int num = 0; //var global que indica las posiciones de la tabla de cada filosofo

// char *S[CANT_FILOSOFOS] = {"Garberoglio","HM","Agodio","Ortiz","Mogni"};

// int philosopherTable()
// {
    
//     sem_open("mutex", 1,0); //ni idea como va

//     for (int i = 0; i < CANT_FILOSOFOS; i++)
//         sem_open(S[i], 0, 0);

//     for (int i = 0; i < CANT_FILOSOFOS; i++)
//         filosofo();
// }