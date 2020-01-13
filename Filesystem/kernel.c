/*
Team: 2-8
Members: Jerry Zheng, Jocelynn Cheesebourough 
*/

#include "kernel.h"

int strlen(char *s);
void *memcpy(void *dest, void *src, int n);
int memcmp(void *s1, void *s2, int n);

void printNum(int num);

void helloWorld();
void printChar(char c);
void printString(char *s);
void readString(char *line);
int mod(int a, int b);
int div(int a, int b);
void readSector(char *buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void readFile(char *filename, char *buffer);
void executeProgram(char *name, int segment);
void terminate();

void main()
{
    char shell[6];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';

    makeInterrupt21();
    interrupt(0x21, 4, shell, 0x2000, 0);
}

/* Modified from: https://clc-wiki.net/wiki/strlen */
int strlen(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        ;
    return i;
}

/* Modified from: https://clc-wiki.net/wiki/memcpy */
void *memcpy(void *dest, void *src, int n)
{
    char *dp = dest;
    char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
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

void printNum(int num)
{
    int digit[5];
    int i;
    int len;

    len = 0;
    while (num > 0)
    {
        digit[len++] = mod(num, 10);
        num = div(num, 10);
    }
    for (i = len - 1; i >= 0; i--)
    {
        printChar(digit[i] + '0');
    }
}

void helloWorld()
{
    /* Prints Hello World to the Screen*/
    putInMemory(0xB000, 0x8000, 'H');
    putInMemory(0xB000, 0x8002, 'e');
    putInMemory(0xB000, 0x8004, 'l');
    putInMemory(0xB000, 0x8006, 'l');
    putInMemory(0xB000, 0x8008, 'o');
    putInMemory(0xB000, 0x800A, ' ');
    putInMemory(0xB000, 0x800C, 'W');
    putInMemory(0xB000, 0x800E, 'o');
    putInMemory(0xB000, 0x8010, 'r');
    putInMemory(0xB000, 0x8012, 'l');
    putInMemory(0xB000, 0x8014, 'd');
}

void printChar(char c)
{
    char al = c;
    char ah = 0xe;
    int ax = ah * 256 + al;
    interrupt(0x10, ax, 0, 0, 0);
}

void printString(char *s)
{
    while (*s != '\0')
    {
        if (*s == '\n')
        {
            printChar('\r');
        }
        printChar(*s);
        s++;
    }
}

void readString(char *line)
{
    int i, lineLength, ax;
    char charRead, backSpace, enter;
    lineLength = 80;
    i = 0;
    ax = 0;
    backSpace = 0x8;
    enter = 0xd;
    charRead = interrupt(0x16, ax, 0, 0, 0);
    while (charRead != enter && i < lineLength - 2)
    {
        if (charRead != backSpace)
        {
            interrupt(0x10, 0xe * 256 + charRead, 0, 0, 0);
            line[i] = charRead;
            i++;
        }
        else
        {
            i--;
            if (i >= 0)
            {
                interrupt(0x10, 0xe * 256 + charRead, 0, 0, 0);
                interrupt(0x10, 0xe * 256 + '\0', 0, 0, 0);
                interrupt(0x10, 0xe * 256 + backSpace, 0, 0, 0);
            }
            else
            {
                i = 0;
            }
        }
        charRead = interrupt(0x16, ax, 0, 0, 0);
    }
    line[i] = 0xa;
    line[i + 1] = 0x0;

    /* correctly prints a newline */
    printString("\n");

    return;
}

int mod(int a, int b)
{
    int temp;
    temp = a;
    while (temp >= b)
    {
        temp = temp - b;
    }
    return temp;
}

int div(int a, int b)
{
    int quotient;
    quotient = 0;
    while ((quotient + 1) * b <= a)
    {
        quotient++;
    }
    return quotient;
}

void readSector(char *buffer, int sector)
{
    int relative_sector, head, track;
    char ah, al, ch, cl, dh, dl;
    int ax, bx, cx, dx;

    relative_sector = mod(sector, 18) + 1;
    head = mod(div(sector, 18), 2);
    track = div(sector, 36);

    ah = 0x2;
    al = 0x1;
    bx = buffer;
    ch = track;
    cl = relative_sector;
    dh = head;
    dl = 0;

    ax = 256 * ah + al;
    cx = 256 * ch + cl;
    dx = 256 * dh + dl;

    interrupt(0x13, ax, bx, cx, dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    if (ax == 0)
    {
        printString(bx);
    }
    else if (ax == 1)
    {
        readString(bx);
    }
    else if (ax == 2)
    {
        readSector(bx, cx);
    }
    else if (ax == 3)
    {
        readFile(bx, cx);
    }
    else if (ax == 4)
    {
        executeProgram(bx, cx);
    }
    else if (ax == 5)
    {
        terminate();
    }
    else
    {
        // printString("handleInterrup21: Operation not supported.");
    }
}

void readFile(char *filename, char *buffer)
{
    char sector_buffer[SECTOR_SIZE];
    struct file_entry *cur_file_entry;
    char *cur_sector_num;
    int filename_len;

    /* Load the directory sector into a 512 byte character array using readSector. */
    readSector(sector_buffer, DIR_SECTOR);

    /* Go through the directory trying to match the file name.  If you do not find it, return. */
    filename_len = strlen(filename);
    for (cur_file_entry = sector_buffer; cur_file_entry->filename[0] != 0x0 && cur_file_entry - (struct file_entry *)sector_buffer < FILE_ENTRY_IN_SECTOR; cur_file_entry++)
    {
        if (memcmp(cur_file_entry->filename, filename, filename_len) == 0)
        {
            for (cur_sector_num = cur_file_entry->sector_num; cur_sector_num != 0x0 && cur_sector_num - cur_file_entry->sector_num < FILE_ENTRY_SECTOR_NUM_SIZE; cur_sector_num++)
            {
                readSector(buffer, *cur_sector_num);
                buffer += SECTOR_SIZE;
            }
            break;
        }
    }
}

void executeProgram(char *name, int segment)
{
    char program_buffer[MAX_PRGM_SIZE];
    int i;

    readFile(name, program_buffer);

    for (i = 0; i < MAX_PRGM_SIZE; i++)
    {
        putInMemory(segment, 0x0000 + i, program_buffer[i]);
    }

    launchProgram(segment);
}

void terminate()
{
    char shell[6];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';
    interrupt(0x21, 4, shell, 0x2000, 0);
}