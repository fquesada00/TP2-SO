#include "../Keyboard_Driver/keyboardDriver.h"
#include "../Video_Driver/video_driver.h"
#include <stdint.h>
#include <lib.h>
#include "States.h"
extern void _hlt();
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
   putsN(buffer,n);
}
int syscall_registers(uint64_t * regs)
{
    State * savedState = getRegs();
    memcpy(regs,savedState,sizeof(State));
    return 1;    
}
