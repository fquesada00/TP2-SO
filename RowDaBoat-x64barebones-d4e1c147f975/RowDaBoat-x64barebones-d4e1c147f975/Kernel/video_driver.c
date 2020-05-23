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
static  const struct vbe_mode_info_structure *screen_info = (void *)0x5C00;
static struct vbe_mode_info_structure * currentscreen_info= (void *)0x5C00;
static uint32_t start =0xFD000000;
char *getAbsolutePixelDataPosition(int x, int y)
{
    return (char *)(start + (x + y * WIDTH) * 3);
}
char *getRelativePixelDataPosition(int x, int y)
{
    return (char *)(currentscreen_info->framebuffer + (x + y * WIDTH) * 3);
}

void write_pixel(int x, int y,char r,char g, char b)
{
    char *pos = getAbsolutePixelDataPosition(x, y);
    pos[0] = b;   //azul
    pos[1] = g;   //verde
    pos[2] = r; //rojo
}
void printChar(char c)
{
    char *bitmap = font8x8_basic[c];
    int set;
    if((currentscreen_info->framebuffer-start)%(WIDTH*3) == 0 && currentscreen_info->framebuffer != start)
        currentscreen_info->framebuffer+=(8*WIDTH*3);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            set = bitmap[i] & (1 << j);
            if (set)
            {
                char *pos = getRelativePixelDataPosition(j, i);
                pos[0] = 255;
                pos[1] = 255;
                pos[2] = 255;
            }
        }
    }
    currentscreen_info->framebuffer += 24;
}

void deleteChar()
{
    if(currentscreen_info->framebuffer == start)
        return;
    if((currentscreen_info->framebuffer-start)%(WIDTH*3) == 0)
    {
        currentscreen_info->framebuffer-=WIDTH*3*8;
    }
    currentscreen_info->framebuffer-=24;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            char *pos = getRelativePixelDataPosition(j, i);
            pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0;
        }
    }
}

void upLine(int line)
{
    if(line != 0)
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < WIDTH;j++)
            {

                char * lineSrc = getAbsolutePixelDataPosition(j,i+line);
                write_pixel(j,i+line-1,lineSrc[2],lineSrc[1],lineSrc[0]);
            }
        }
    }
}

void clear()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
        {
            write_pixel(j,i,0,0,0);
        }
    
    currentscreen_info->framebuffer = start;
}

void clearLine(int line)
{
    for (int i = 0; i < 8 ; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            write_pixel(j,i+(line*9),0,0,0);
        }
    }
}

void puts(char *string)
{
    while (*string != 0)
    {
        printChar(*string);
        string++;
    }
}

void newLine(){

    uint32_t auxi =(screen_info->framebuffer - start)%(WIDTH*3);
    currentscreen_info->framebuffer+=(WIDTH*3) - auxi ;

}
void aux(int d,char * buffer)
{
    buffer[0] = d%10 + '0';
    buffer[1]=0;
    return buffer;
}
void debug()
{
    for(int i = 0;i < HEIGHT;i++)
    {
        if(i%3==0)
            write_pixel(0,i,255,0,0);
        else if(i%3 ==1)
            write_pixel(0,i,0,255,0);
        else
            write_pixel(0,i,0,0,255);
    }
}
