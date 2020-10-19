#ifndef _FD_H_
#define _FD_H_
#include <stddef.h>
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
typedef enum type
{
    STDINOUT,
    PIPE
} type;

typedef struct file
{
    char *read;
    char *write;
    int reading;
    int writing;
    int *idxRead;
    int *idxWrite;
    int id;
    type type;
} file_t;
void init_fds();
void closeCurrentProcess(int fd);
void closePID(size_t pid, int fd);
#endif