#include <stdio.h>
#include <stdlib.h>

#define CTE (size_t)1000000 * 4      //store 4mb, recordemos que mi memoria tiene que ser una potencia de dos, sino nos vamos de manos
#define MINIMUN_BLOCK_SIZE (size_t)8 //8 bytes
#define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/
#define WORD_ALIGN_MASK 7

 //GASTON: RECORDA SIEMPRE QUE ESTE ALGORITMO ES EFICIENTE POR OPERACION DE BIT DE DECALAJE, NO USES DIVISIONES, IMBECIL.

typedef struct TREE_NODE
{
	struct TREE_NODE * father;
    struct TREE_NODE * leftChild;
    struct TREE_NODE * rightChild;
    size_t blockSize;
    char blocked;
} treeNode;

static const size_t headerSize = (sizeof(treeNode) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK);

static size_t remainingBytes = CTE;

static treeNode * root;

void * pMalloc(size_t bytes);
void pInitHeap(void * heapStart,size_t memSize);
void pFree(void *);
void pInsertBlockIntoList(a_block *);

void pInitHeap(void * heapStart,size_t memSize) {
	//No valido parametros porque es una funcion interna, es mas, nisiqueira se porque me paso memSize es CTE
	//Aunque si las constantes dependen del SO tal vez podria validar... no se

	//Creo que el tamaño ya lo tengo, sino lo puedo mandar al principio del heap

	//Tambien creo que estas tres son al pedo porque static se inicializa en cero no?
	root->father = NULL;
	root->leftChild = NULL;
	root->rightChild = NULL;
	root->blockSize = CTE;
	root->blocked = 0; 
	return;
}

void pFree(void * pointer){
	/*
		La logica es la siguiente
			1.Le digo a mi bloque que estoy libre
			2.Me fijo si mi hermano esta libre
			3aSi esta libre le digo a mi padre que esta libre y repito recursivamente.
			3b. Si no estoy libre returneo.
	*/

	//Me apunta a mi nodo o ahi esta guardado el nodo?
	treeNode * node = (treeNode *)pointer;
	node->blocked = 0; 
	if( (node->father != NULL) && !node->father->leftChild->blocked && !node->father->rightChild->blocked) {
		node->father->blocked = 0;
		pFree((void *)(node->father));
	}
	return;
}

