#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_
#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);

void test_mm(int argc,char * argv[]);
void test_processes(int argc,char * argv[]);
void test_prio(int argc,char * argv[]);
void test_sync(int argc,char * argv[]);
void test_no_sync(int argc,char * argv[]);


#endif