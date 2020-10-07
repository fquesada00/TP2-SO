#include "keyboardDriver.h"
#include <stdint.h>
#include "../include/fds.h"
extern FILE_DESCRIPTOR *fds[TOTAL_FDS];
extern char getKeyboardScanCode();
char stdin[256];
static int buff_size = 0;
static int current = 0;

static const int latinasccode[0x56][3] =
    {
        {0, 0, 0}, {0, 0, 0}, {'1', '!', '|'}, {'2', '"', '@'}, {'3', '#', 0}, {'4', '$', 0}, {'5', '%', 0}, {'6', '&', 0}, {'7', '/', '{'}, {'8', '(', '['}, {'9', ')', ']'}, {'0', '=', '}'}, {'\'', '?', '\\'}, {0, 0, 0}, {'\b', '\b', 0}, {'\t', '\t', 0}, {'q', 'Q', '@'}, {'w', 'W', 0}, {'e', 'E', 0}, {'r', 'R', 0}, {'t', 'T', 0}, {'y', 'Y', 0}, {'u', 'U', 0}, {'i', 'I', 0}, {'o', 'O', 0}, {'p', 'P', 0}, {0, 0, 0}, {'+', '*', 0}, {'\n', '\n', 0}, {0, 0, 0}, {'a', 'A', 0}, {'s', 'S', 0}, {'d', 'D', 0}, {'f', 'F', 0}, {'g', 'G', 0}, {'h', 'H', 0}, {'j', 'J', 0}, {'k', 'K', 0}, {'l', 'L', 0}, {0, 0, 0}, {'{', '[', 0}, {'|', 0, 0}, {0, 0, 0}, {'}', ']', 0}, {'z', 'Z', 0}, {'x', 'X', 0}, {'c', 'C', 0}, {'v', 'V', 0}, {'b', 'B', 0}, {'n', 'N', 0}, {'m', 'M', 0}, {',', ';', 0}, {'.', ':', 0}, {'-', '_', 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {' ', ' ', ' '}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {'<', '>', '|'}
    };

void keyboardHandler(uint64_t rsp)
{
    signed char scan = getKeyboardScanCode();
    static int shift = 0;
    static int control = 0;
    //Los codigos de los scan code de apretar una tecla van de 0 a 0x56
    if (scan >= 0 && scan <= 0x56)
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
            if (scan == 0x02)
            {
                //changeScreen(0);
                //continueProgram(0, rsp);
            }
            else if (scan == 0x03)
            {
                //changeScreen(1);
                //continueProgram(1, rsp);
            }
            else if (scan == 0x13)
            {
                // save_regs(rsp);
            }
        }
        else
        {
            fds[1]->buffer[(fds[1]->idxWrite)++] = (signed char)latinasccode[scan][shift];
            putChar(latinasccode[scan][shift]);
        }
    }
    //Release code shift
    else if (scan == 0xFFFFFFFFFFFFFFAA || scan == 0xFFFFFFFFFFFFFFB6 || scan == 0xFFFFFFFFFFFFFFB8)
    {
        shift = 0;
    }
    else if (scan == 0xFFFFFFFFFFFFFF9D)
    {
        control = 0;
    }

    //Buffer circular
    if (fds[1]->idxWrite == BUFFER_SIZE)
        fds[1]->idxWrite = 0;
}

char getBuffer(){
    char c = fds[1]->buffer[(fds[1]->idxRead)++];
    if(fds[1]->idxRead == fds[1]->idxWrite) return -1;
    if(fds[1]->idxRead == BUFFER_SIZE) fds[1]->idxRead = 0;
    return c;
}