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
    sleep(10000);


    return 0;

}