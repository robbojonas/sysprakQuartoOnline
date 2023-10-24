#include "think.h"

void think_(int sig)
{
    (void)sig;
    if (serverinfo ->startcalc == 1)
    {
        printfield();
        // calculate best move .....
    }
}
