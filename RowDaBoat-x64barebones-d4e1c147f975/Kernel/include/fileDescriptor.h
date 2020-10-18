#ifndef FD_H
#define FD_H
#define MAX_FD 120
#define BUF_SIZE 1024
typedef enum type{
    STDINOUT,
    PIPE
}type;
typedef struct file{
    char * read;
    char * write;
    int reading;
    int writing;
    int *idxRead;
    int *idxWrite;
    int id;
    type type;
} file_t;

#endif