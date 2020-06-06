#include "../Exceptions/Zero_Division.h"
#include "../Exceptions/Invalid_Op_Code.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6

static void zero_division(uint64_t);
static void invalid_op_code(uint64_t);

void exceptionDispatcher(int exception,uint64_t rsp) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division(rsp);
	else if(exception == INVALID_OP_CODE)
		invalid_op_code(rsp);
}


static void zero_division(uint64_t rsp) {
	zero_division_handler(rsp);
}

//TODO
static void invalid_op_code(uint64_t rsp){
	invalid_op_code_handler(rsp);
}