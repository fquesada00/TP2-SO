#include "idle.h"
#include "interrupts.h"
int idle_pid;
void idle()
{
    while (1)
    {
        _hlt();
    }
}