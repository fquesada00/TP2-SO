#include "standardlib.h"

void shell()
{
    char command[256]={0};
    int i = 0;
    char c = 0;
    int n,l,k;
    int le = 10;
    while (strcmp(command,"quit") != 0)
    {
        command[0]=0;
        //printf("\nUser:> ");
        i=0;
        /*while(1){
            scanf("%c",&c);
            printf("Letra ingresada: %c\n",c);
            while(le){
                printf("\n \n \n");
                le--;
            }
            le = 10;
        }*/
        while(1){
            printf("\nUser:> ");
            scanf("(%d+%d",&n,&l);
            printf("Cuenta: (%d+%d)*%d",n,l,k);
            /*while(le){
                printf("\n \n \n");
                le--;
            }
            le = 10;*/
       }
        
        while ((c=getchar()) != '\n')
        {
            if(c != '\b'){
                command[i++] = c;
                putchar(c);
            }else if(i > 0){
                i--;
                putchar(c);
            }
        }
        putchar('\n');
        command[i]=0;
        if(strcmp(command,"processor") == 0)
            processorInfo();
        else if(strcmp(command,"printmem") == 0)
        {
            printMemoryFromAddress(0x5C00);
        }
    }
    
}