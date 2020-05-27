#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H_
#include <stdint.h>
void write_pixel(int x, int y,char r,char g, char b);
void printChar(char c);
void puts(char * string);
void newLine();
void clearLine(int line);
void clear();
void aux(int d,char * buffer);
void debug();
void deleteChar();
void cursor();
#endif