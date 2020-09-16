#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
#include <stdint.h>
void invalid_op_code_handler(uint64_t);
void zero_division_handler(uint64_t);
#endif