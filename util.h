#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <stdlib.h>

#define BUFMAX 50
#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

void die(const char *fmt, ...);
int cmpint(const void *lhs, const void *rhs);
int indexof(const int *key, const int *base, size_t nmemb);
#endif //UTIL_H
