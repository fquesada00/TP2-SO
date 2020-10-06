#ifndef _FDS_H_
#define _FDS_H_
#include <stdint.h>
#include <stddef.h>
#define TOTAL_FDS 512
#define PROCESSES 50
#define BUFFER_SIZE 256

typedef struct{
    size_t pids[PROCESSES];
    size_t idxWrite;
    size_t idxRead;
    char buffer[BUFFER_SIZE];

}FILE_DESCRIPTOR;

#endif