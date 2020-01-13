#include "shell.h"

void main()
{

    char line[MAX_LINE_LEN];

    while (1)
    {
        interrupt(0x21, 0, "SHELL> ");
        interrupt(0x21, 1, line);
        interrupt(0x21, 0, "\n");
        interrupt(0x21, 0, "Bad Command!\n");
    }
}