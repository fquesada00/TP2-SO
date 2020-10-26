#ifndef _SYS_LIB_H_
#define _SYS_LIB_H_
#include <stdint.h>
#include <stddef.h>

extern int syswrite(int fd, const char *buff, int n);
extern int sysread(int fd, char *buff, int bytes);
extern int read_mem(uint64_t address, char *buff);
#endif


