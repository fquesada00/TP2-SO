/* sampleCodeModule.c */
#include "../standardlib.h"
extern void shell();
char fun();
typedef void (*function)();
typedef struct
{
	function programa;
	int screen;
} ProgramType;

int main()
{
	shell();
}
char fun()
{
	return (char)'a';
}