#include "file_work.hpp"

void read_file(const char* filepath, BufInfo* buf_info)
{
    FileInfo file_info;
    file_info.fp = fopen(filepath, "rb"); //read binary
    if (file_info.fp == NULL)
    {
        perror("fopen");
        printf("Tried to open: \"%s\"\n", filepath);
        exit(1);
    }

    struct stat file_stat;
    stat(filepath, &file_stat);
    file_info.sz = file_stat.st_size;


    buf_info->sz = file_info.sz; //not adding '\0'
    buf_info->buf = (char*)malloc(buf_info->sz);

    fread(buf_info->buf, 1, file_info.sz, file_info.fp);
}

