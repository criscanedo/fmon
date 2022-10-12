#ifndef HANDLES_H
#define HANDLES_H
#include <sys/stat.h>

struct file_info {
    char *pathname;
    long fpos;
    off_t size;
};

void setinfo(struct file_info *info, const char *pathname, size_t len);
void printlines(struct file_info *info);

#endif
