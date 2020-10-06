#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){

    int myPipe[2];

    if((pipe(myPipe))<0){
        printf("Error creating pipe.\n");
        exit(EXIT_FAILURE);
    }    

    printf("Descriptors are %d and %d\n", myPipe[0], myPipe[1]);

    int myPipe2[2];

    if((pipe(myPipe2))<0){
        printf("Error creating pipe.\n");
        exit(EXIT_FAILURE);
    }    

    printf("Descriptors are %d and %d\n", myPipe2[0], myPipe2[1]);
    char buffer[256];
    write(myPipe2[1],"chau",4);
    write(myPipe2[1],"hola como",9);
    read(myPipe2[0],buffer,15);
    printf("%s\n",buffer);
    read(myPipe2[0],buffer,2);
    printf("%s\n",buffer);
    write(myPipe2[1],"hola como",9);
    read(myPipe2[0],buffer,256);
    printf("%s\n",buffer);

    return 0;

}