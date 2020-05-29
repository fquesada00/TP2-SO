#include "../Exceptions/Zero_Division.h"
#include "../Exceptions/Invalid_Op_Code.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6

static void zero_division();
static void invalid_op_code();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else if(exception == INVALID_OP_CODE)
		invalid_op_code();
}


static void zero_division() {
	zero_division_handler();
}

//TODO
static void invalid_op_code(){
	invalid_op_code_handler();
}