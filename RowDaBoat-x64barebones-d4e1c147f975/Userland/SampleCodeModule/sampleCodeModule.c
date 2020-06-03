/* sampleCodeModule.c */
#include "../standardlib.h"
extern void shell();
typedef void (*function)();
typedef struct
{
	function programa;
	int screen;
} ProgramType;

int main()
{
	
	shell();
	return 0;
}