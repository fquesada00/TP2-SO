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
	shell();
	//calc();
	return 0;
}