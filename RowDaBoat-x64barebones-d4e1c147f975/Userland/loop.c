#include "standardlib.h"
void loop()
{
    while (1)
    {
        printf("loop1");
        for (int i = 0; i < 100000000; i++)
            ;
    }
}