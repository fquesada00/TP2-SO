#include "shell.h"
#include "processLib.h"
#include "semLib.h"
#include "pipeLib.h"
#include "sysLib.h"
#include <stdint.h>
#include "stdioLib.h"
#include "stringLib.h"
void help()
{
    printf("\n\n\t\t\t\t\t\t\t\t\t\tSHELL COMMANDS LIST\n\n\n");
    printf("\tinforeg: MUESTRA POR SALIDA ESTANDARD EL VALOR DE LOS\n\tREGISTROS\n\n");
    printf("\t\t");
    printf("Presionar CTRL + R para guardar los registros en el momento\n\tdeseado y luego ejecutar el comando\n\n");
    printf("\tprintmem: RECIBE UNA DIRECCION DE MEMORIA Y REALIZA UN VOLCADO\tDE MEMORIA DE 32 BYTES A PARTIR DE LA DIRECCION INDICADA\n\n");
    printf("\ttime: MUESTRA POR SALIDA ESTANDARD LA HORA DEL SISTEMA\n\n");
    printf("\tprocessor: MUESTRA POR SALIDA ESTANDARD LA MARCA, MARCA\n\tEXTENDIDA, FAMILIA Y MODELO DEL PROCESADOR\n\n");
    printf("\ttemp: MUESTRA POR SALIDA ESTANDARD LA TEMPERATURA DEL\n\tPROCESADOR MEDIDA EN GRADOS CENTIGRADOS\n\n");
    printf("\ttestDivZero: EJECUTA LA EXCEPCION 0 DE TIPO DIVISION POR CERO\n\n");
    printf("\t\tMuestra el funcionamiento de la rutina de atencion \n\tde dicha excepcion\n\n");
    printf("\ttestInvOpCode: EJECUTA LA EXCEPCION 6 DE TIPO INVALID OP CODE\n\n");
    printf("\t\tMuestra el funcionamiento de la rutina \n\tde atencion de dicha excepcion\n\n");
    printf("\thelp: MUESTRA POR SALIDA ESTANDAR ESTE PANEL\n\n");
    printf("\n\n\n");
    printf("\tPARA EJECUTAR CUALQUIER COMANDO, SE DEBE PRESIONAR LA TECLA\n\tENTER AL FINAL\n");
    printf("\n\n\n");
    printf("\tALUMNOS: Quesada, Francisco y Serpe, Octavio\n");
}

void ps(void)
{
    printProcesses();
    return;
}



int kill(int pid){
    if(pid <= 0)
        return -1;
    return pKill(pid);

}
int block(int pid, int block)
{
    if (block > 1 || block < 0 || pid <= 0)
        return -1;
    return pBlock(pid, block);
}

void nice(int pid, int p){
    if(pid <= 0 || p < 0 || p > MAXPRIVILAGE)
        return;
    _nice(pid,p);
}

void sem(){
    semPrint();
    return;
}
void pipe()
{
    pipePrint();
    return;
}
void myPid(){
    char pidStr[MAXLENGHT] = {0};
    int pid = _getPID();
    uintToBase(pid,pidStr,10);
    puts(pidStr);
}