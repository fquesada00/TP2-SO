#include "../Keyboard_Driver/keyboardDriver.h"
#include "../Video_Driver/video_driver.h"
extern void _hlt();
int syscall_read(int fd, char * buffer,int n)
{
    for (int i = 0; i < n; i++)
    {
        while(is_buffer_empty())
            putChar('a');
        if(!is_buffer_empty())
            buffer[i]=get_buffer();
    }
}