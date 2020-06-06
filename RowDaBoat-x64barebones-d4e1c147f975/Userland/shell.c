#include "standardlib.h"
extern int syswrite(int fd, const char *buff, int bytes);
void shell()
{
    printf("\nArranco la shell\n");
    char command[256]={0};
    int i = 0;
    char c = 0;
    int n,l,k;
    int le = 10;
    double d=0;
    char * p;
    /*processorInfo();
    while(1){
    printf("\nIngrese numero:");
    scanf("%f",&d);
    
    printf("\nNumero leido: %f\n",d);
    break;
    }
    return;*/
    while (strcmp(command,"quit") != 0)
    {
        command[0]=0;
        printf("\nUser:> ");
        i=0;
        
        /*while(1){
            printf("\nUser:> ");
            //scanf("%d",&n);
            //scanf("%f",&d);
            printf("\nNumber read: %f",d);
            //printf("%d\n",n);
            //scanf("%d+%d*%d",&n,&l,&k);
            //printf("Cuenta: %d+%d*%d",n,l,k);
            while(le){
                printf("\n \n \n");
                le--;
            }
            le = 10;
       }
        */
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
        else if(strcmp(command,"inforeg")==0)
        {
            printReg();
        }
        else if(strcmp(command,"divZero")==0)
        {
            DivZero();
        }
        else if(strcmp(command,"invOpCode") == 0)
        {
            invOpCode();
        }
        else if(strcmp(command,"time")==0)
        {
            printRtc();
        }else{
            printf("'%s' is not a valid command\n",command);
        }
    }
    
}