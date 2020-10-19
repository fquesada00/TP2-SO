#include "sysLib.h"
#include "stdioLib.h"
void printMemoryFromAddress(uint64_t address)
{
    char buff[255] = {0};
    if (address < 0)
    {
        printf("\nERROR: %d is not a valid address\n", address);
        return;
    }
    unsigned char *p = (unsigned char *)address;
    read_mem(address, buff);
    for (int i = 0; i < 32; i++)
    {
        printf("%d = %d\n", p, buff[i]);
        p++;
    }
}