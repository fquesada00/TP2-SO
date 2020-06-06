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
int main()
{
	programLoader((function)calc); //left screen
	programLoader((function)shell); //right screen
	printf("Press CTRL + 1 to run calculator on left screen\n");
	printf("Press CTRL + 2 to run shell on right screen\n");
	printf("Waiting for instruction...\n");
	while (1)
	{
		getchar();
	}
	return 0;
}