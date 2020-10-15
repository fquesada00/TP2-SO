
#define ME_RASCO_EL_BOLERO 0
#define ME_PICA_EL_BAGRE 1
#define ME_COMO_LA_VIDA 2
#define CANT_FILOSOFOS 6
#define IZQUIERDA ((fil + CANT_FILOSOFOS - 1) % CANT_FILOSOFOS)
#define DERECHA ((fil + 1) % CANT_FILOSOFOS)
int * table;
int num; //var global que indica las posiciones de la tabla de cada filosofo
int myNum; //mi indice 

sem_t mutex;
sem_t S[CANT_FILOSOFOS];

void * filosofo() {
	myNum = num++;
	int fil = myNum;
	//sem_open
	while(1){
		sleep(1);
		hungry();
		sleep(1);
		finishEating();
		if(talbe[LEFT] == ME_RASCO_EL_BOLERO && talbe[RIGHT] == ME_RASCO_EL_BOLERO){
			printf("Why are we still here? Just to suffer?\n");
		}
	}

}

void hungry(){
	sem_wait(mutex); //espero a ser el unico accediendo a la mesa
	table[myNum] = ME_PICA_EL_BAGRE;
	tryToEat(myNum);
	sem_post(mutex);
	sem_wait(S[myNum]);
	sleep(1);
}

void tryToEat(int fil){
	if(table[fil] == ME_PICA_EL_BAGRE && table[IZQUIERDA] != ME_COMO_LA_VIDA && talbe[DERECHA] != ME_COMO_LA_VIDA) {
		table[fil] = ME_COMO_LA_VIDA;
		sleep(2);
		printTableState();
		sem_post(S[fil]);
	}
}

void finishEating(){
	sem_wait(mutex);
	talbe[myNum] = ME_RASCO_EL_BOLERO;
	int fil = myNum;
	tryToEat(LEFT);
	tryToEat(RIGHT);

	sem_post(mutex);

}

void printTableState(){
	for(int i = 0; i < CANT_FILOSOFOS;i++){
		printf("%d",table[i]);
	}
	printf("\n"); //esto esta fea la impresion pero no se que pasa si me interrumpen, o despues lo cambio de ultima
}