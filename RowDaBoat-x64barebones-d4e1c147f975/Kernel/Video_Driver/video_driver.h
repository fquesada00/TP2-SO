#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H_
#include <stdint.h>
void write_pixel(int x, int y,char r,char g, char b);
void printChar(char c);
void puts(char * string);
void newLine();
void clearLine(int line);
void clear();
void deleteChar();
void upLine(int line);
void scrollUp();
void init_video();
#endif