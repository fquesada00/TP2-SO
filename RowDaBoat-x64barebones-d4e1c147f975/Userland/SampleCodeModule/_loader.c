/* _loader.c */
#include <stdint.h>
#include "standardlib.h"
extern char bss;
extern char endOfBinary;

int shell();

int _start() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	return shell();

}
