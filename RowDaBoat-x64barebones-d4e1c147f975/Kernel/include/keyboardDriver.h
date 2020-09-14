#ifndef KEYBOARDDRIVER_H_
#define KEYBOARDDRIVER_H_
#include <stdint.h>
void keyboardHandler(uint64_t rsp);
int is_buffer_empty();
char get_buffer();
#endif