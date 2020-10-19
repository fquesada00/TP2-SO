#ifndef _STDIO_LIB_H_
#define _STDIO_LIB_H_
#define STDOUT_FILENO 1
#define STDIN_FILENO 0
void vaArg(int n, ...);
void puts(char *c);
int printf(const char *fmt, ...);
int scanf(const char *fmt, ...);
int getchar();
void putchar(char c);
#endif