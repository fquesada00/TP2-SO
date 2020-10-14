#ifndef FD_H
#define FD_H
#define MAX_FD 120
#define BUF_SIZE 1024
typedef struct file{
    char * read;
    char * write;
    int reading;
    int writing;
    int idxR;
    int idxW;
} file_t;
#endif