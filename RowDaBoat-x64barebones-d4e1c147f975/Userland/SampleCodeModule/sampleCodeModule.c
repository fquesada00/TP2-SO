/* sampleCodeModule.c */
#include "standardlib.h"
extern void shell();
typedef void (*function)();
typedef struct ProgramType
{
	function programa;
	int screen;
} ProgramType;
int main()
{

	
	// if( programLoader((function)shell)) //left screen calc, right screen shell
	// {
		// printf("Error Loading Program!!\n");
	// 	// return 1;
	// }
	// printf("Press CTRL + 1 to run calculator on left screen\n");
	// printf("Press CTRL + 2 to run shell on right screen\n");
	// printf("Waiting for instruction...\n");
	// while(1) getchar();
	// return 0;
}