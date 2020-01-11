/*
Team: 2-8
Members: Jerry Zheng, Jocelynn Cheesebourough 
*/

void helloWorld();
void printChar(char c);
void printString(char *s);
void readString(char *line);
int mod(int a, int b);
int div(int a, int b);
void readSector(char *buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);

void main()
{
    char line[80];
    char buffer[512];

    helloWorld();

    printString("World Hello!\r\n");

    printString("Enter a line: \0");
    readString(line);
    printString(line);

    readSector(buffer, 30);
    printString(buffer);

    makeInterrupt21();
    /* interrupt(0x21,0,0,0,0); */

    interrupt(0x21, 1, line, 0, 0);
    interrupt(0x21, 0, line, 0, 0);

    while (1)
        ;
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
    printString("\r\n");

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
    else
    {
        printString("handleInterrup21: Operation not supported.");
    }
}