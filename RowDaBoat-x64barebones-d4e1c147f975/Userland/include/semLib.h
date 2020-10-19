#ifndef _SEM_LIB_H_
#define _SEM_LIB_H_
#include <stddef.h>
extern int semOpen(const char *name, size_t value, char created);
extern int semWait(const char *name);
extern int semPost(const char *name);
extern int semClose(const char *name);
extern void semPrint();
#endif