#include <stdio.h>
#include <stdlib.h>
/*
Al pensar como implementar buddy se nos ocurrieron tres alternativas: 
La primera, utilizaba una combinacion entre bitMap y arbol binario 
Representabamos un arbol (arbol linearizado) con un solo bit que se leia por niveles, de izquierda a derecha, un 1 representaba que estaba lleno y un 0 vacio
Este metodo era probablemente el mas eficiente posible pero era increiblemente compleja el manejo del arbon con bits
No solo eso, sino que ademas no permitia (aunque no se haya pedido implementar) defragmentar el buddy system.
Finalmente teniamos que tener constantemente todo el arbol en memoria, aunque solo sean bits, hacia ruido.
Por estas tres razones, no fuimos por este camino.

Por otro lado se nos ocurrio la opcion de poner un header al principio de cada bloque y cuando lo partiamos
hacer dos nuevos bloques con el nuevo header. Esta opcion era muy intuitiva y facil de programar.
Llegamos a un problema cuando pensamos como guardar el arbol izquierda recursivamente
Seria un gran bloque de header header header ocupando espacio en memoria.
Este si era mas facil de defragmentar, aunque no tanto.

Finalmente, nos iluminamos (hubo un poco de internet aca)
Nos inspiramos en la implementacion de Linux, unos genios
Y hicimos la implementacion actual, donde tenemos un array de listas
donde cada lista es un puntero al tipo de nodo
Este si esta cheto, tenemos la operacion de bit linearizada pero sin tener que siga siendo leyendo
*/

//LINK COPADO https://github.com/evanw/buddy-malloc
#define HEADER 8 //NECESITO 8 BYTES PARA SABER CUANTO OCUPO; ESTO VA EN EL HEAP

#define MINIMUM_BLOCK_SIZE_LOG2  4 //ALGO MENOR ME SACA EL DEALINEAMIENTO A PALABRA
#define MINIMUN_BLOCK_SIZE ((size_t)1 << MINIMUM_BLOCK_SIZE_LOG2) //8 bytes

#define MAXIMUM_BLOCK_SIZE_LOG2 21 // MAXIMO 2MB
#define MAXIMUM_BLOCK_SIZE ((size_t)1 << MAXIMUM_BLOCK_SIZE_LOG2)


#define LEVELS (MAXIMUM_BLOCK_SIZE_LOG2 - MINIMUM_BLOCK_SIZE_LOG2)

#define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/
#define TRUE 1
#define FALSE 0

 //GASTON: RECORDA SIEMPRE QUE ESTE ALGORITMO ES EFICIENTE POR OPERACION DE BIT DE DECALAJE, NO USES DIVISIONES, IMBECIL.

typedef struct TREE_NODE
{
	
    struct TREE_NODE * next;
    //struct TREE_NODE * previous;
    size_t occupied; //hago size t para alinear a palabra
    size_t address;
}treeList; //Este nombre es una estafa, es solo una lista 


static treeList treeByLevels[LEVELS];

void * pMalloc(size_t bytes);
void pInitHeap(void * heapStart,size_t memSize);
void pFree(void *);

void * pMalloc(size_t bytes){
	if(bytes < MINIMUN_BLOCK_SIZE)
		return NULL; //O algun excepecion
	
	int level = findLevel(bytes)
	if(level == -1)
		return NULL; //o algun error

	//me fijo si hay alguna lista libre

	treeList currentNode = treeByLevels[level];
	while(currentNode != NULL && currentNode.occupied) {
		currentNode = currentNode.next;
	}
	if(currentNode != NULL) {
		currentNode.occupied = TRUE;
		return currentNode.address + HEADER;
	}

	else{
		//Busco algun padre que tenga espacio, si ninguno lo tiene, pregunto en su padre, etc. si no, no hay memoria
		for(int i = level - 1; i >= 0; i--){ //recorro a los padres
			currentNode = treeByLevels[level] //Tengo el primer de la lista
			while(currentNode != NULL && currentNode.occupied) {
				currentNode = currentNode.next;
			}
			if(currentNode != NULL) 
				createChilds(currentNode,i, level); //Crea recursivamente desde el indice i hasta el indice level
			}

		}
	}

}

void createChilds(treeList currentNode,int from, int to) {
	
}

int findLevel(size_t bytes){
	int ans = 0;
	size_t memoryPartition = MAXIMUM_BLOCK_SIZE;

	if(bytes > memoryPartition)
		return -1;

	while(ans < LEVELS) {
		memoryPartition = memoryPartition >> 1;
		if(bytes > memoryPartition)
			return ans;
		ans++;
	}
}
