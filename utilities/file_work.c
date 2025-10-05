#include "file_work.h"

void read_file(const char* filepath, BufInfo* buf_info)
{
    FileInfo file_info;
    file_info.fp = fopen(filepath, "rb"); //read binary

    struct stat file_stat;
    stat(filepath, &file_stat);
    file_info.sz = file_stat.st_size;

    buf_info->sz = file_info.sz + 1; //add '\0'
    buf_info->buf = (char*)malloc(sizeof(buf_info->sz));

    fread(buf_info->buf, 1, file_info.sz, file_info.fp);
}

