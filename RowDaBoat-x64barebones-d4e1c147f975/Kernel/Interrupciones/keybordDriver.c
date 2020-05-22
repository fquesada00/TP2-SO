#include "keyboradDriver.h"
#include "../video_driver.h"
#include "../include/naiveConsole.h"
extern unsigned char getKeyboardScanCode();

static const char asccode[58][2] = 
{
    {0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'},
    {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('},
    {'0', ')'}, {'-', '_'}, {'-', '+'}, {'\b', '\b'}, {'\t', '\t'}, 
    {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, 
    {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, 
    {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, 
    {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'°', '~'}, 
    {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, 
    {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}
};
static const char special[2] = {0,0};
void keyboardHandler()
{
    char scan = getKeyboardScanCode();
    static int shift=0;
    if(scan >=0){
        if(scan == 42)
        {
            shift=1;
            keyboardHandler();
        }else if(scan == 14)
        {
            deleteChar();
        }
        else{
            printChar(asccode[scan][shift]);
            shift=0;
        }
    }


}