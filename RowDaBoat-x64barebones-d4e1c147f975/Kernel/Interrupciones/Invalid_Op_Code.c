#include "Exceptions.h"
#include "../Video_Driver/video_driver.h"
#include "../Interrupciones/States.h"
extern void restartProgram(uint64_t);
void invalid_op_code_handler(uint64_t rsp){
    State * currentState = (void*) rsp;
    putsColor("INVALID OP CODE EXCEPTION\n",255,0,0);
    printreg(*currentState);
    restartProgram(rsp);
}