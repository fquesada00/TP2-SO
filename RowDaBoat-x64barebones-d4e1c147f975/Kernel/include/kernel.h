#ifndef _KERNEL_H_
#define _KERNEL_H_
void * initializeKernelBinary();
void * getStackBase();
void clearBSS(void * bssAddress, uint64_t bssSize);
extern int initProcessManager(void *entry_point, int argc, char *argv[], int fg);
#endif