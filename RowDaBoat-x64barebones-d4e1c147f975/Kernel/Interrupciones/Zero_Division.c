#include "Exceptions.h"
#include "../Video_Driver/video_driver.h"
#include "../Interrupciones/States.h"
extern void save_regs(uint64_t);
extern void restartProgram(uint64_t);
void zero_division_handler(uint64_t rsp)
{
    State * currentState = (void*)rsp;
    putsColor("ZERO DIVISION EXCEPTION\n", 255, 0, 0);
    printreg(*currentState);
    restartProgram(rsp);
}
