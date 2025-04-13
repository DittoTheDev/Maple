#ifndef GETLINE_H
#define GETLINE_H

#include <stdio.h>
#include <stdlib.h>

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
typedef long ssize_t;
#endif

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#endif
