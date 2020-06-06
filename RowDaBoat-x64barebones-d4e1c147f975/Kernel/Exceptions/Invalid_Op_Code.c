#include "Invalid_Op_Code.h"
#include "../Video_Driver/video_driver.h"
//TODO
extern void printreg(int,uint64_t);
extern void save_regs(uint64_t);
extern uint64_t * getRegs();
extern void restartProgram(uint64_t);
void invalid_op_code_handler(uint64_t rsp){
    save_regs(rsp);
    uint64_t * buffer = getRegs();
    putsColor("INVALID OP CODE EXCEPTION\n",255,0,0);
    for(int i = 15;i>0;i--)
    {
        printreg(i,buffer[i-1]);
    }
    restartProgram(rsp);
}