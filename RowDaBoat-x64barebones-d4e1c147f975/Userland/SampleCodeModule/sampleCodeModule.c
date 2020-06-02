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

<<<<<<< HEAD
int main() {
	printf("\n \n \n \n \n \n \n");
	printf("\n \n \n \n \n \n \n");
	printf("\n \n \n \n \n \n \n");
	printf("\n \n \n \n \n \n \n");
	int n;
	scanf("%d",&n);
	printf("tu numero es %d y deberias haber escrito 2",n);
	printf("\n \n \n \n \n \n \n");
	printf("\n \n \n \n \n \n \n");
	printf("\n \n \n \n \n \n \n");
	printf("\n \n \n \n \n \n \n");
=======
int main()
{
	shell();
}
char fun()
{
	return (char)'a';
>>>>>>> 0abf163e986ff192622ba3d022df991cc7966920
}