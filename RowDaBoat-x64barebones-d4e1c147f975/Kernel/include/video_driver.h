#ifndef _VIDEO_DRIVER_H
#define _VIDEO_DRIVER_H_
#include <stdint.h>
#define SCREENS 1
#define WIDTH 1024
#define SCREEN_WIDTH (WIDTH / SCREENS)
#define HEIGHT 768
/*Si desea cambiar el tamaño de la fuente por favor cargue un nuevo bitmap*/
#define CHARSIZE 8
#define PIXELSIZE 3
#define LINES (HEIGHT / CHARSIZE)
#define COLS ((WIDTH) / CHARSIZE)
#define START_POS WIDTH *PIXELSIZE *CHARSIZE *(LINES - 1)
#define START_SCREEN 0
void write_pixel(int x, int y, char r, char g, char b);
void putChar(char c);
void puts(const char *string);
void newLine();
void clearLine(int line);
void clear();
void deleteChar();
void upLine(int line);
void scrollUp();
void init_video();
void changeScreen(int num);
int putsN(const char *buffer, int n);
void putsColor(const char *string, char r, char g, char b);
void putCharColor(char c, char r, char g, char b);
int screenNumber();
#endif