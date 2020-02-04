/*
Team: 2-8
Members: Jerry Zheng, Jocelynn Cheesebourough 
*/
#ifndef KERNEL_H
#define KERNEL_H

#define MAP_SECTOR 1
#define DIR_SECTOR 2

#define SECTOR_SIZE 512
#define FILE_ENTRY_SIZE 32
#define FILE_ENTRY_FILENAME_SIZE 6
#define FILE_ENTRY_SECTOR_NUM_SIZE 26
#define FILE_ENTRY_IN_SECTOR 16

#define RECORDS_IN_MAP_SECTOR 256

#define MAX_PRGM_SIZE 13312

struct file_entry
{
    char filename[6];
    char sector_num[26];
};

#define PROCESS_TABLE_SIZE 8
#define PROCESS_TABLE_STARTING_SEGMENT 0x2000
#define PROCESS_TABLE_ENTRY_SIZE 0x1000

struct process_table_entry
{
    int active; /* 1=active, 0=inactive,2=waiting */
    int sp;
    int wait_on;
};

#endif