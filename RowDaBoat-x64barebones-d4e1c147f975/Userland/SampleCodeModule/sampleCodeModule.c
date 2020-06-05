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
{
<<<<<<< HEAD
	printf("Start");
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
=======
	int d;
	double f;
	char s[256] = {0};
	int c;
	d = 15;
	f = 125.369;
	printf("\nes un numero: %d, letra: %c, string: %s, float: %f\n",123, 'k', "hola mundo", 123.456);
	
	while ((1))
	{
		printf("\nIngrese la cuenta:");
		scanf("%s=",s);
		printf("\nRESULTADO: %s\n",s);
		putchar('\n');
	}
	
	/*while ((1))
	{	
		printf("\nUser $ >");
		int val = scanf("f: %f", &f);
		printf("ret val: %d\n",val);
		printf("pusiste: %f\n", f);
	}

	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	int ret = scanf("%s=%d=%c=%f", &s, &d, &c, &f);
	if (ret)
	{
		printf("everything ok");
		printf("\n%s=%d=%c=%f", s, d, c, f);
	};
	printf("ret value %d", ret);
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf(s);
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n");
	while (1)
>>>>>>> e0cfee0399baaa6f494f35bf05c917bb78376525
	{
		if(current_screen == calculatorP.screen)
			calculatorP.programa();
		else if(current_screen == shellP.screen)
			shellP.programa();
	}
	
	
	return 0;
}