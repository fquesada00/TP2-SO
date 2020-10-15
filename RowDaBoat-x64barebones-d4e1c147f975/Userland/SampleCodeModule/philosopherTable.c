
#define ME_RASCO_EL_BOLERO 0
#define ME_PICA_EL_BAGRE 1
#define ME_COMO_LA_VIDA 2
#define CANT_FILOSOFOS 6
#define IZQUIERDA ((fil + CANT_FILOSOFOS - 1) % CANT_FILOSOFOS)
#define DERECHA ((fil + 1) % CANT_FILOSOFOS)
int * table;
int num; //var global que indica las posiciones de la tabla de cada filosofo

sem_t mutex;
sem_t S[CANT_FILOSOFOS];


int philosopherTable(){
	sem_init(mutex,0,1); //ni idea como va

	for(int  i = 0; i < CANT_FILOSOFOS;i++)
		sem_init(S[i],0,0);

	for(int i = 0; i < CANT_FILOSOFOS; i++)
		filosofo();
}