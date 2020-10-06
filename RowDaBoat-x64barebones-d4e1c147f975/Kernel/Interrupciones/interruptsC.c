#include "keyboardDriver.h"
#include "video_driver.h"
#include "scheduler.h"
#include "list.h"
#include <stdint.h>
#include <lib.h>
extern void _hlt();
extern Header readyHeader;
int syscall_read(int fd, char * buffer,int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        while(is_buffer_empty())
        {
            _hlt();
        }
        if(!is_buffer_empty())
            buffer[i]=get_buffer();
    }

    return i;
}
int syscall_write(int fd, const char * buffer,int n)
{
   return putsN(buffer,n);
}
int syscall_registers(uint64_t * regs)
{
   // State * savedState = getRegs();
    //memcpy(regs,savedState,sizeof(State));
    return 0;    
}

int syscall_read_mem(uint64_t address, char * buff)
{
    memcpy((void *) buff,(void *) address,32);
    return 0;
}

int pKill(int pid)
{
    elem_t e;
    e.PID = pid;
    removeElement(&readyHeader,e);
    return 0;
}

