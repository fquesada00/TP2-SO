#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_
#include <stddef.h>
#include "fds.h"
int syscall_read(int fd, char *buffer, int n);
int syscall_write(int fd, const char *buffer, int n);
void writeToStd(char * buff,size_t n, file_t *f);
#endif