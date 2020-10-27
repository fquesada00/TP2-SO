// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "keyboardDriver.h"
#include "list.h"
#include "syscalls.h"
#include "ctes.h"
extern file_t *stdout;
extern file_t *stdin;

static char buff[BUFF_SIZE];
static int buff_current = 0;
static const int latinasccode[0x56][3] =
    {
        {0, 0, 0}, {0, 0, 0}, {'1', '!', '|'}, {'2', '"', '@'}, {'3', '#', 0}, {'4', '$', 0}, {'5', '%', 0}, {'6', '&', 0}, {'7', '/', '{'}, {'8', '(', '['}, {'9', ')', ']'}, {'0', '=', '}'}, {'\'', '?', '\\'}, {0, 0, 0}, {'\b', '\b', 0}, {'\t', '\t', 0}, {'q', 'Q', '@'}, {'w', 'W', 0}, {'e', 'E', 0}, {'r', 'R', 0}, {'t', 'T', 0}, {'y', 'Y', 0}, {'u', 'U', 0}, {'i', 'I', 0}, {'o', 'O', 0}, {'p', 'P', 0}, {0, 0, 0}, {'+', '*', 0}, {'\n', '\n', 0}, {0, 0, 0}, {'a', 'A', 0}, {'s', 'S', 0}, {'d', 'D', 0}, {'f', 'F', 0}, {'g', 'G', 0}, {'h', 'H', 0}, {'j', 'J', 0}, {'k', 'K', 0}, {'l', 'L', 0}, {0, 0, 0}, {'{', '[', 0}, {'|', 0, 0}, {0, 0, 0}, {'}', ']', 0}, {'z', 'Z', 0}, {'x', 'X', 0}, {'c', 'C', 0}, {'v', 'V', 0}, {'b', 'B', 0}, {'n', 'N', 0}, {'m', 'M', 0}, {',', ';', 0}, {'.', ':', 0}, {'-', '_', 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {' ', ' ', ' '}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {'<', '>', '|'}};

void keyboardHandler()
{
    signed char scan = getKeyboardScanCode();
    static int shift = 0;
    static int control = 0;
    //Los codigos de los scan code de apretar una tecla van de 0 a 0x56
    if (scan >= 0 && scan < 0x56)
    {
        //scan code de los shifts
        if (scan == 0x2A || scan == 0x36)
        {
            shift = 1;
        }
        //Alt
        else if (scan == 0x38)
        {
            shift = 2;
        }
        //Block Mayusc
        else if (scan == 0x3A)
        {
            shift = !shift;
        }
        else if (scan == 0x1D)
        {
            control = 1;
        }
        else if (control)
        {
            if (scan == 0x20)
            {
                writeToBuff(EOF);
            }
        
        }
        else
        {
            writeToBuff((signed char)latinasccode[scan][shift]);
        }
    }
    //Release code shift
    else if (scan == (signed long)0xFFFFFFFFFFFFFFAA || scan == (signed long)0xFFFFFFFFFFFFFFB6 || scan == (signed long)0xFFFFFFFFFFFFFFB8)
    {
        shift = 0;
    }
    else if (scan == (signed long)0xFFFFFFFFFFFFFF9D)
    {
        control = 0;
    }

    //Buffer circular
    if (*stdin->idxWrite >= BUFF_SIZE)
        *stdin->idxWrite = 0;
}

void writeToBuff(char c)
{
    char auxi[2] = {0};
    if (c != '\n' && c != '\b' && c != EOF)
    {
        auxi[0] = c;
        writeToStd(auxi, 1, stdout);
        buff[buff_current++] = c;
    }
    else if (c == '\b' && buff_current != 0)
    {
        buff_current--;
        auxi[0] = c;
        writeToStd(auxi, 1,stdout);
    }
    else if (c == '\n' || c == EOF)
    {
        buff[buff_current++] = c;
        if (c != EOF)
        {
            auxi[0] = c;
            writeToStd(auxi, 1, stdout);
        }
        writeToStd(buff, buff_current, stdin);
        buff_current = 0;
    }
}
