/* sampleCodeModule.c */
#include "../standardlib.h"
extern void shell();char fun();
typedef void(*function)();
typedef struct{
	function programa;
	int screen;
} ProgramType;

int main() {
	int buffer[256]={0};
	int c=0;
	int i = 0;
	c = getchar();
	printf("FUERA");
	while (c!='\n')
	{
		printf("Dentro");
		putchar(c);
		buffer[i++]=c;
		c=getchar();
	}
	buffer[i]=0;
	printf("\n%d\n",strcmp(buffer,"printmem"));
	printf("%d\n",strcmp(buffer,"processor"));

	
	/*
	while (1)
	{
		shell();
		printf("Exit Shell\n");
	}*/
	return 0;
	

}
char fun()
{
	return (char)'a';
}