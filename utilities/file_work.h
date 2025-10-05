#include <stdio.h>
#include <sys/stat.h>

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

#endif
