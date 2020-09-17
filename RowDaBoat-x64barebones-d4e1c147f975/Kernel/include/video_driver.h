#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H_
#include <stdint.h>
void write_pixel(int x, int y,char r,char g, char b);
void putChar(char c);
void puts(const char * string);
void newLine();
void clearLine(int line);
void clear();
void deleteChar();
void upLine(int line);
void scrollUp();
void init_video();
void changeScreen(int num);
int putsN(const char * buffer, int n);
void putsColor(const char *string, char r, char g, char b);
void putCharColor(char c, char r, char g, char b);
int screenNumber();
#endif