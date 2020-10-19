#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_
#include <stdint.h>
#define EOF -1
void keyboardHandler(uint64_t rsp);
void writeToBuff(char c);
extern char getKeyboardScanCode();
#endif