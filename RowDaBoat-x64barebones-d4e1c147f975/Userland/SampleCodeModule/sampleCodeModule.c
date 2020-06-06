/* sampleCodeModule.c */
#include "../standardlib.h"
extern void shell();
extern void calc();
typedef void (*function)();
typedef struct ProgramType
{
	function programa;
	int screen;
} ProgramType;
static int current_screen;
int main()
{/*
	printf("Start");
	shell();
	ProgramType shellP;
	ProgramType calculatorP;
	shellP.programa=(function)shell;
	calculatorP.programa=(function)calc;
	printf("Before getscrenn");
	calculatorP.screen=getScreen();
	shellP.screen=getScreen();
	printf("before while");
	printf("Program cal screen: %d\nProgram shellscreen: %d\n ",calculatorP.screen,shellP.screen);
	while (1)	
	{
		if(current_screen == calculatorP.screen)
			calculatorP.programa();
		else if(current_screen == shellP.screen)
			shellP.programa();
	}
	
	*/
	programLoader((function)calc);
	programLoader((function)shell);
	printf("Waiting to run...");
	while (1)
	{
		getchar();
	}
	
	return 0;
}