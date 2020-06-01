#include "../Keyboard_Driver/keyboardDriver.h"
extern void _hlt();
int syscall_read(int fd, char * buffer,int n)
{
    for (int i = 0; i < n; i++)
    {
        while(is_buffer_empty())
            _hlt();
        buffer[i]=get_buffer();
    }
    
}