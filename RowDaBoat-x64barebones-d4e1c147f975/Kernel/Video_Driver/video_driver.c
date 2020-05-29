#include <stdint.h>
#include "video_driver.h"
#include "font8x8_basic.h"
#define MARGIN 8
#define WIDTH 1024
#define HEIGHT 680
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

/*Obtiene un puntero al pixel deseado a partir de la posicion inicial*/
char *getAbsolutePixelDataPosition(int x, int y)
{
    return (char *)(start + (x + y * WIDTH) * 3);
}

/*Obtiene un puntero al pixel deseado a parti de la posicion actual de donde se esta escribiendo*/
char *getRelativePixelDataPosition(int x, int y)
{
    return (char *)(currentscreen_info->framebuffer + (x + y * WIDTH) * 3);
}


/*Pinta un pixel a partir de una direccion absoluta*/
void write_pixel(int x, int y, char r, char g, char b)
{
    char *pos = getAbsolutePixelDataPosition(x, y);
    pos[0] = b; //azul
    pos[1] = g; //verde
    pos[2] = r; //rojo
}

/*Escribe el caracter a la posicion actual de la pantalla y aumenta el current*/
void printChar(char c)
{
    //Accedo al mapa de bits de la fuente y tomo el mapa del caracter deseado
    char *bitmap = font8x8_basic[c];
    int set;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            set = bitmap[i] & (1 << j);
            if (set)
            {
                char *pos = getRelativePixelDataPosition(j, i);
                //Pinto de color Blanco los pixeles
                pos[0] = 255;
                pos[1] = 255;
                pos[2] = 255;
            }
        }
    }
    //Si estoy al final de la linea pongo mi current en la siguiente linea de caracteres
    if (((currentscreen_info->framebuffer) - start) % (WIDTH * 3) == (WIDTH - 8) * 3)
    {
        currentscreen_info->framebuffer += (8 * WIDTH * 3);
    }
    //Aumento el current en 1 caracter
    currentscreen_info->framebuffer += 24;
}

/*Borro el caracter en la posicion anterior a mi current*/
void deleteChar()
{
    //Si estoy al comienzo de todo no hago nada
    if (currentscreen_info->framebuffer == start)
        return;
    //Si estoy al comienzo de una linea que no es la primera me muevo una linea de caracteres para arriba
    if ((currentscreen_info->framebuffer - start) % (WIDTH * 3) == 0)
    {
        currentscreen_info->framebuffer -= WIDTH * 3 * 8;
    }
    //Me voy un caracter para atras
    currentscreen_info->framebuffer -= 24;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            char *pos = getRelativePixelDataPosition(j, i);
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
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {

                char *lineSrc = getAbsolutePixelDataPosition(j, i + line*8);
                write_pixel(j, i + (line-1)*8, lineSrc[2], lineSrc[1], lineSrc[0]);
            }
        }
    }
}

/*Vacia la pantalla*/
void clear()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
        {
            write_pixel(j, i, 0, 0, 0);
        }

    currentscreen_info->framebuffer = start;
}
//TOCHECK
/*Elimina una linea*/
void clearLine(int line)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            write_pixel(j, i + (line * 8), 0, 0, 0);
        }
    }
}
void scrollUp()
{
    int i;
    for(i = 1;i<96;i++)
        upLine(i);
    clearLine(i-1);
}
/*Imprime un string a pantalla*/
void puts(char *string)
{
    while (*string != 0)
    {
        printChar(*string);
        string++;
    }
}
//CAMBIE DE NUEVO EL 9 por el 8
/*Imprime un salto de Linea*/
void newLine()
{

    uint32_t auxi = (currentscreen_info->framebuffer - start) % (WIDTH * 3);
    currentscreen_info->framebuffer += (WIDTH * 3 * 8) - auxi;
}
int current_screen;

int screenNumber(){
    return current_screen;
}
