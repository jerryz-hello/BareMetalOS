#include "shell.h"
#include "kernel.h"

int strlen(char *s);
int memcmp(void *s1, void *s2, int n);

void main()
{
    char line[MAX_LINE_LEN];
    char file_buffer[MAX_PRGM_SIZE];
    char *filename;

    while (1)
    {
        interrupt(0x21, 0, "SHELL> ", 0, 0);
        interrupt(0x21, 1, line, 0, 0);
        interrupt(0x21, 0, "\n", 0, 0);

        /* removing the enter at the end */
        line[strlen(line) - 1] = '\0';

        if (memcmp(line, "type", 4) == 0)
        {
            for (filename = line + 4; (*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;
            
            /* clear the buffer */
            file_buffer[0]='\0';

            interrupt(0x21, 3, filename, file_buffer, 0);

            interrupt(0x21, 0, file_buffer, 0, 0);
        }
        else
        {
            interrupt(0x21, 0, "Bad Command!\n", 0, 0);
        }
    }
}

/* Modified from: https://clc-wiki.net/wiki/memcmp */
int memcmp(void *s1, void *s2, int n)
{
    unsigned char *p1 = s1, *p2 = s2;
    while (n--)
        if (*p1 != *p2)
            return *p1 - *p2;
        else
            p1++, p2++;
    return 0;
}

/* Modified from: https://clc-wiki.net/wiki/strlen */
int strlen(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        ;
    return i;
}