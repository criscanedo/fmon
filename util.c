#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

void die(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (fmt[0] && fmt[strnlen(fmt, BUFMAX) - 1] == ':') {
        fputc(' ', stderr);
        perror(NULL);
    } else {
        fputc('\n', stderr);
    }

    exit(EXIT_FAILURE);
}

int cmpint(const void *lhs, const void *rhs)
{
    if (*(const int *)lhs < *(const int *)rhs) return -1;
    else if (*(const int *)rhs < *(const int *)lhs) return 1;
    return 0;
}

int indexof(const int *key, const int *base, size_t nmemb)
{
    int *p = (int *)bsearch(key, base, nmemb, sizeof(int), cmpint);
    return !p ? -1 : p - base;
}
