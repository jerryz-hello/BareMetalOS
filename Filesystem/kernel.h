#ifndef KERNEL_H
#define KERNEL_H

#define DIR_SECTOR 2

#define SECTOR_SIZE 512
#define FILE_ENTRY_SIZE 32
#define FILE_ENTRY_FILENAME_SIZE 6
#define FILE_ENTRY_SECTOR_NUM_SIZE 26
#define FILE_ENTRY_IN_SECTOR 16

#define MAX_PRGM_SIZE 13312

struct file_entry
{
    char filename[6];
    char sector_num[26];
};

#endif