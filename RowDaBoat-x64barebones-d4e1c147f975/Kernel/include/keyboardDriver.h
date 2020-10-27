#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_
#define EOF -1
void keyboardHandler();
void writeToBuff(char c);
extern char getKeyboardScanCode();
void int_21();
#endif