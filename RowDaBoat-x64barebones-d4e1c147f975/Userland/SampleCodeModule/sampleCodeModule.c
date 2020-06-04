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
	char s[64] = {0};
	int c;
	d = 15;
	f = 125.369;
	while ((1))
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