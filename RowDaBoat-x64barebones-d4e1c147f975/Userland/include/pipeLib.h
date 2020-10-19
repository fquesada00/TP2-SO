#ifndef _PIPE_LIB_H_
#define _PIPE_LIB_H_
#define R 0
#define W 1
/*Max args for a piped command*/
#define ARGSPIPE 1
extern int pipeClose(int fds[2], const char *name);
extern int initProcessWithPipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode, int fg);
extern int pipeOpen(int fd[2], const char *name);
extern void pipePrint(void);

#endif