#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#ifndef FILE_WORK_HEADER
#define FILE_WORK_HEADER

typedef struct
{
    FILE* fp;
    size_t sz;
} FileInfo;    
    
typedef struct 
{
    char* buf;
    size_t sz;
    size_t pos;
} BufInfo;

void read_file(const char* filepath, BufInfo* buf_info);

#endif
