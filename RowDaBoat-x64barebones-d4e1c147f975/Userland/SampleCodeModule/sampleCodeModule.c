/* sampleCodeModule.c */
#include "../standardlib.h"
extern void shell();
extern void calc();
typedef void (*function)();
typedef struct
{
	function programa;
	int screen;
} ProgramType;

int main()
{
	int d;
	double f;
	char s[64] = "Hola";
	int c;
	d = 15;
	f = 125.369;
	/*while (1)
	{

		c = getchar();
		printf("\nEsto es un string sin vars\n");
		printf("tiene un int %d\n", d);
		printf("string %s\n", s);
		printf("double %f\n", f);
		printf("char %c\n", c);
	}*/
	//shell();
	calc();
	return 0;
}