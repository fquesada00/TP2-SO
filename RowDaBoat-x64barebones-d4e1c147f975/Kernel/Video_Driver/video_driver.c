#include <stdint.h>
#include "video_driver.h"
#include "font8x8_basic.h"
#define SCREENS 2
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
struct vbe_mode_info_structure
{
    uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;     // deprecated
    uint8_t window_b;     // deprecated
    uint16_t granularity; // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;        // number of bytes per horizontal line
    uint16_t width;        // width in pixels
    uint16_t height;       // height in pixels
    uint8_t w_char;        // unused...
    uint8_t y_char;        // ...
    uint8_t planes;
    uint8_t bpp;   // bits per pixel in this mode
    uint8_t banks; // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__((packed));

/*Estructura para escribir pixeles en pantalla*/
static struct vbe_mode_info_structure *currentscreen_info = (void *)0x5C00;

/*Posicion de inicio de la pantalla*/
static uint32_t start = 0xFD000000;
static int current[SCREENS] = {START_POS};
static int current_screen = START_SCREEN;
static int chars_written[SCREENS] = {0};
static int screens_in_use;
void init_video()
{
    for (int j = 0; j < SCREENS - 1; j++)
    {
        for (int i = 0; i < HEIGHT; i++)
            write_pixel(WIDTH / SCREENS + j * SCREEN_WIDTH, i, 255, 255, 255);
    }
    for (int i = 0; i < SCREENS; i++)
    {
        current[i] = START_POS;
        chars_written[i] = 0;
    }
}

/*Obtiene un puntero al pixel deseado a partir de la posicion inicial*/
uint32_t getAbsolutePixelDataPosition(int x, int y)
{
    return (start + (x + y * WIDTH) * PIXELSIZE);
}

/*Obtiene un puntero al pixel deseado a parti de la posicion actual de donde se esta escribiendo*/
uint32_t getRelativePixelDataPosition(int x, int y)
{
    return (currentscreen_info->framebuffer + current[current_screen] + (SCREEN_WIDTH + 1) * current_screen * PIXELSIZE + (x + y * WIDTH) * PIXELSIZE);
}

/*Pinta un pixel a partir de una direccion absoluta*/
void write_pixel(int x, int y, char r, char g, char b)
{
    char *pos = (char *)(getAbsolutePixelDataPosition(x, y));
    pos[0] = b; //azul
    pos[1] = g; //verde
    pos[2] = r; //rojo
}

/*Escribe el caracter a la posicion actual de la pantalla y aumenta el current*/
void putChar(char c)
{
    putCharColor(c, 255, 255, 255);
}
/*Escribe el caracter a la posicion actual de la pantalla con un color RGB y amenta el current*/
void putCharColor(char c, char r, char g, char b)
{
    if (r > 255 || g > 255 || b > 255)
        return;
    if (c == '\n')
        newLine();
    else if (c == '\b')
        deleteChar();
    else if (c == '\t')
        puts("  ");
    else if (c == 1)
        changeScreen(0);
    else if (c == 2)
        changeScreen(1);
    else
    {
        //Accedo al mapa de bits de la fuente y tomo el mapa del caracter deseado
        char *bitmap = font8x8_basic[c];
        int set;

        for (int i = 0; i < CHARSIZE; i++)
        {
            for (int j = 0; j < CHARSIZE; j++)
            {
                set = bitmap[i / (CHARSIZE / 8)] & (1 << j / (CHARSIZE / 8));
                if (set)
                {
                    char *pos = (char *)(getRelativePixelDataPosition(j, i));
                    //Pinto de color Blanco los pixeles
                    pos[0] = b;
                    pos[1] = g;
                    pos[2] = r;
                }
            }
        }
        //Si estoy al final de la linea pongo mi current en la siguiente linea de caracteres
        if ((/*(currentscreen_info->framebuffer) - start)*/ current[current_screen] % (SCREEN_WIDTH * PIXELSIZE) == (SCREEN_WIDTH - CHARSIZE) * PIXELSIZE))
        {
            scrollUp();
            ///*currentscreen_info->framebuffer*/current[current_screen] += (CHARSIZE * WIDTH * PIXELSIZE) - SCREEN_WIDTH * PIXELSIZE;
            current[current_screen] = START_POS - CHARSIZE * PIXELSIZE;
        }
        //Aumento el current en 1 caracter
        /*currentscreen_info->framebuffer*/ current[current_screen] += PIXELSIZE * CHARSIZE;
        chars_written[current_screen]++;
    }
}

/*Borro el caracter en la posicion anterior a mi current*/
void deleteChar()
{
    //Si estoy al comienzo de todo no hago nada
    if (/*currentscreen_info->framebuffer == start current[current_screen] == START_POS*/ chars_written[current_screen] <= 0)
        return;
    //Si estoy al comienzo de una linea que no es la primera me muevo una linea de caracteres para arriba
    if (/*(currentscreen_info->framebuffer - start) % (WIDTH * 3) == 0*/ current[current_screen] % SCREEN_WIDTH * PIXELSIZE == 0)
    {
        /*currentscreen_info->framebuffer*/ current[current_screen] -= WIDTH * PIXELSIZE * CHARSIZE - SCREEN_WIDTH * 3;
    }
    //Me voy un caracter para atras
    //currentscreen_info->framebuffer -= 24;
    current[current_screen] -= PIXELSIZE * CHARSIZE;
    chars_written[current_screen]--;
    for (int i = 0; i < CHARSIZE; i++)
    {
        for (int j = 0; j < CHARSIZE; j++)
        {
            char *pos = (char *)(getRelativePixelDataPosition(j, i));
            //"Borro" el pixel
            pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0;
        }
    }
}
/*Copia la linea deseada a la posicion superior. Funcion auxiliar para el scroll*/
void upLine(int line)
{
    if (line != 0)
    {
        for (int i = 0; i < CHARSIZE; i++)
        {
            for (int j = (SCREEN_WIDTH)*current_screen; j < SCREEN_WIDTH * (current_screen + 1); j++)
            {

                char *lineSrc = (char *)(getAbsolutePixelDataPosition(j, i + line * CHARSIZE));
                write_pixel(j, i + (line - 1) * CHARSIZE, lineSrc[2], lineSrc[1], lineSrc[0]);
            }
        }
    }
}

/*Vacia la pantalla*/
void clear()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = (SCREEN_WIDTH)*current_screen; j < SCREEN_WIDTH * (current_screen + 1); j++)
        {
            write_pixel(j, i, 0, 0, 0);
        }

    /*currentscreen_info->framebuffer = start;*/ current[current_screen] = START_POS;
}
//TOCHECK
/*Elimina una linea*/
void clearLine(int line)
{
    for (int i = 0; i < CHARSIZE; i++)
    {
        for (int j = SCREEN_WIDTH * current_screen; j < SCREEN_WIDTH * (current_screen + 1); j++)
        {
            write_pixel(j, i + (line * CHARSIZE), 0, 0, 0);
        }
    }
}
void scrollUp()
{
    int i;
    for (i = 1; i < LINES; i++)
        upLine(i);
    clearLine(i - 1);
}
/*Imprime en pantalla un string con color*/
void putsColor(char *string, char r, char g, char b)
{
    while (*string != 0)
    {
        putCharColor(*string,r,g,b);
        string++;
    }
}

/*Imprime un string a pantalla*/
void puts(char *string)
{
    while (*string != 0)
    {
        putChar(*string);
        string++;
    }
}
//CAMBIE DE NUEVO EL 9 por el 8
/*Imprime un salto de Linea*/
void newLine()
{
    /*
    uint32_t auxi = /*(currentscreen_info->framebuffer - start) % (WIDTH * 3);*/
    ;
    /*currentscreen_info->framebuffercurrent[current_screen] += (WIDTH * 3 * 8) - auxi;*/
    scrollUp();
    current[current_screen] = START_POS;
    chars_written[current_screen] = 0;
}

void putsN(char *buffer, int n)
{
    for (int i = 0; i < n && buffer[i]; i++)
    {
        putChar(buffer[i]);
    }
    return;
}

int screenNumber()
{
    if (screens_in_use >= SCREENS)
        return -1;
    return screens_in_use++;
}
void changeScreen(int num)
{
    if (num >= SCREENS)
        return;
    current_screen = num;
}

