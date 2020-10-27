// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
    printf("\thelp: imprimir comandos\n\n");
    printf("\tmem: imprimir estado memoria\n\n");
    printf("\tps: imprimir estado procesos\n\n");
    printf("\tloop: proceso loop\n\n");
    printf("\tkill: matar PID --> kill <PID>\n\n");
    printf("\tnice: cambiar privilegio de PID --> nice <PID> <PRIV> (0<= PRIV <=10)\n\n");
    printf("\tblock: bloquear proceso --> block <PID> <1|0> (1 block, 0 unblock)\n\n");
    printf("\tsem: imprimir estado semaforos\n\n");
    printf("\tcat: lee de stdin e imprime a stdout. Finaliza con EOF\n\n");
    printf("\twc: lee de stdin y cuenta lineas y bytes. Finaliza con EOF\n\n");
    printf("\tfilter: lee de stdin y filtra las vocales. Finaliza con EOF\n\n");
    printf("\tpipe: imprimir estado pipes\n\n");
    printf("\tescribo : leo --> Muestra recepcion y escritura por entrada estandar\n\n");
    printf("\tphylo con R remueve un filosofo y con A agrega\n\n");
    printf("\ttest_mm : test memory manager\n\n");
    printf("\ttest_processes: test processes\n\n");
    printf("\ttest_prio: test priority\n\n");
    printf("\ttest_sync: test synchro\n\n");
    printf("\ttest_no_sync: -\\_(!-!)_/-\n\n");
    printf("\tleo: lee 15 bytes de stdin\n\n");
    printf("\tescribo: escribe 'hola como estas'\n\n");
    printf("\tnanananananaLider\n\n");
    printf("\t\n\n");

    printf("\tpuede decidir correr en foreground o background con un &\n\n");
    printf("\n\n\n");
    printf("\t con CTRL + D para enviar un EOF, mucho poder conlleva mucha reponsabilidad\n\n");
    printf("\n\n\n");
    printf("\tpara ejecutar cualqueir comando, se debe preisonar la tecla\n\tENTER\n");
    printf("\n\n\n");
    printf("\tAlumnos: Quesada Francisco, Serpe Octavio y Donikian Gaston\n");
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

void display_welcome_message(void){
    printf("                                  _     _     _     _     _     _     _     _     _     _     _     _   \n");
	printf("                                 (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)  \n");
	printf("                                  / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\   \n");
	printf("                                __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ \n");
	printf("                               (_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._)\n");
	printf("                                  || o ||     || s ||     || i ||     || t ||     || O ||     || S ||   \n");
	printf("                                _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ \n");
	printf("                               (.-./`-'\\.-.(.-./`-`\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-`\\.-.)\n");
	printf("                                `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' \n");     
    printf("                   _     _     _     _     _     _     _     _     _     _     _     _     _     _     _     _   \n");
	printf("                  (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)  \n");
	printf("                   / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\   \n");
	printf("                 __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ \n");
	printf("                (_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._)\n");
	printf("                   || S ||     || I ||     || S ||     || T ||     || E ||     || M ||     || A ||     || S ||   \n");
	printf("                 _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ \n");
	printf("                (.-./`-`\\.-.(.-./`-'\\.-.(.-./`-`\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-`\\.-.)\n");
	printf("                 `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' \n");
    printf("          _     _     _     _     _     _     _     _     _     _     _     _     _     _     _     _     _     _     _     _   \n");
	printf("         (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)  \n");
	printf("          / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\   \n");
	printf("        __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ \n");
	printf("       (_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._)\n");
	printf("          || O ||     || P ||     || E ||     || R ||     || A ||     || T ||     || I ||     || V ||     || O ||     || S ||   \n");
	printf("        _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ \n");
	printf("       (.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-`\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-`\\.-.)\n");
	printf("        `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' \n\n\n");
    printf("                                        _     _     _     _     _     _     _     _     _     _     _     _   \n");
	printf("                                       (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)   (c).-.(c)  \n");
	printf("                                        / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\     / ._. \\   \n");
	printf("                                      __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ __\\( Y )/__ \n");
	printf("                                     (_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._(_.-/'-'\\-._)\n");
	printf("                                        || F ||     || Q ||     || O ||     || S ||     || G ||     || D ||   \n");
	printf("                                      _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ _.' `-' '._ \n");
	printf("                                     (.-./`-'\\.-.(.-./`-`\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-'\\.-.(.-./`-`\\.-.)\n");
	printf("                                      `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' `-'     `-' \n");    
    printf("					      						 		   Trabajo Practico\n");
    printf("						      					 		  Sistemas Operativos\n");
    printf("							      				 		2do Cuatrimestre - 2020\n\n");
	printf("							      						  Welcome to ositOS\n");
	printf("								     	Type \"help\" to discover all available commands\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

// void secret(){
//     printf("                                                                   /L'-,\n");
//     printf("                               ,'-.           /MM . .             /  L '-,\n");
//     printf("     .                    _,--dMMMM\\         /MMM  `..           /       '-, \n");
//     printf("     :             _,--,  )MMMMMMMMM),.      `QMM   ,<>         /_      '-,'\n");
//     printf("     ;     ___,--. \\MM(    `-'   )M//MM\\       `  ,',.;      .-'* ;     .'\n");
//     printf("     |     \\MMMMMM) \\MM\\       ,dM//MMM/     ___ < ,; `.      )`--'    /\n");
//     printf("     |      \\MM()M   MMM)__   /MM(/MP'  ___, \\  \\ `  `. `.   /__,    ,'\n");
//     printf("     |       MMMM/   MMMMMM( /MMMMP'__, \\     | /      `. `-,_\\     /\n");
//     printf("     |       MM     /MMM---' `--'_ \\     |-'  |/         `./ .\\----.___\n");
//     printf("     |      /MM'   `--' __,-  \\""   |-'  |_,               `.__) . .F. )-.\n");
//     printf("     |     `--'       \\   \\    |-'  |_,     _,-/            J . . . J-'-. `-.,\n");
//     printf("     |         __  \\`. |   |   |         \\    / _           |. . . . \\   `-.  F\n");
//     printf("     |   ___  /  \\  | `|   '      __  \\   |  /-'            F . . . . \\     '`   \n");
//     printf("     |   \\  \\ \\  /  |        __  /  \\  |  |,-'        __,- J . . . . . \\         \n");
//     printf("     |    | /  |/     __,-  \\  ) \\  /  |_,-     __,--'     |. .__.----,'        \n");
//     printf("     |    |/    ___     \\    |'.  |/      __,--'           `.-;;;;;;;;;\\       \n");
//     printf("     |     ___  \\  \\     |   |  `   __,--'                  /;;;;;;;;;;;;. \n");
//     printf("     |     \\  \\  |-'\\    '    __,--'                       /;;;;;;;;;;;;;;\\ \n");
//     printf(" \\   |      | /  |      __,--'                             `--;;/     \\;-'\\  \n");
//     printf("  \\  |      |/    __,--'                                   /  /         \\  \\  \n");
//     printf("   \\ |      __,--'                                        /  /           \\  \\ \n");
//     printf("    \\|__,--'                                          _,-;M-K,           ,;-;\\ \n");
//     printf("                                                     <;;;;;;;;           '-;;;;\n");

// }