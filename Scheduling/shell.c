/*
Team: 2-8
Members: Jerry Zheng, Jocelynn Cheesebourough 
*/
#include "shell.h"
#include "kernel.h"

int strlen(char *s);
int memcmp(void *s1, void *s2, int n);
void *memcpy(void *dest, void *src, int n);

void main()
{
    int i;
    int file_size;
    char line[MAX_LINE_LEN];
    char file_buffer[MAX_PRGM_SIZE];
    char *filename;
    char *pid;
    char pid1[7];
    int pid_len;
    int pid_n;
    struct file_entry *cur_file_entry;
    char sector_buffer[SECTOR_SIZE];
    char cur_filename[7];
    char filename1[7];
    char *filename1_ptr;
    int filename1_len;

     enableInterrupts();
     

    /* shell*/
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
            file_buffer[0] = '\0';

            interrupt(0x21, 3, filename, file_buffer, 0);

            interrupt(0x21, 0, file_buffer, 0, 0);
        }
        else if (memcmp(line, "execute", 7) == 0)
        {
            for (filename = line + 7; (*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;
            interrupt(0x21, 4, filename, 0, 0);
        }
        else if (memcmp(line, "dir", 3) == 0)
        {
            interrupt(0x21, 2, sector_buffer, DIR_SECTOR, 0);
            for (cur_file_entry = sector_buffer; cur_file_entry->filename[0] != 0x0 && cur_file_entry - (struct file_entry *)sector_buffer < FILE_ENTRY_IN_SECTOR; cur_file_entry++)
            {
                memcpy(cur_filename, cur_file_entry->filename, FILE_ENTRY_FILENAME_SIZE);
                cur_filename[FILE_ENTRY_FILENAME_SIZE] = '\0';
                interrupt(0x21, 0, cur_filename, 0, 0);
                interrupt(0x21, 0," ",0,0);
                for(file_size=0;cur_file_entry->sector_num[file_size]&&file_size<FILE_ENTRY_SECTOR_NUM_SIZE;file_size++)
                ;
                interrupt(0x21,11,file_size,0,0);
                interrupt(0x21, 0, "\n", 0, 0);
            }
        }
        else if (memcmp(line, "copy", 4) == 0)
        {
            for (filename = line + 4; (*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;
            filename1_ptr = filename;
            for (; !(*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;
            filename1_len = filename - filename1_ptr;
            memcpy(filename1, filename1_ptr, filename1_len);
            filename1[filename1_len] = '\0';

            interrupt(0x21, 3, filename1, file_buffer, 0);

            for (; (*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;
            filename1_ptr = filename;
            for (; !(*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;
            filename1_len = filename - filename1_ptr;
            memcpy(filename1, filename1_ptr, filename1_len);
            filename1[filename1_len] = '\0';

            interrupt(0x21, 8, filename1, file_buffer, 3);
        }
        else if (memcmp(line,"kill",4)==0){
            for (pid = line + 4; (*pid == ' ' || *pid == '\t') && *pid != '\0'; pid++)
                ;
                pid_n=*pid-'0';
            interrupt(0x21,9,pid_n,0,0);
        }
        else if(memcmp(line,"quit",4)==0){
            interrupt(0x21,9,0,0,0);
        }
        else if(memcmp(line,"execforeground",14)==0){
            for (filename = line + 14; (*filename == ' ' || *filename == '\t') && *filename != '\0'; filename++)
                ;

            interrupt(0x21,12,filename,0,0);
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

/* Modified from: https://clc-wiki.net/wiki/memcpy */
void *memcpy(void *dest, void *src, int n)
{
    char *dp = dest;
    char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
}
