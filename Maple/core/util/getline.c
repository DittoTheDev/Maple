#include "getline.h"
#include <errno.h>

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        errno = EINVAL;
        return -1;
    }
    size_t pos = 0;
    int c = 0;
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL)
            return -1;
    }
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            *n *= 2;
            char* new_ptr = realloc(*lineptr, *n);
            if (new_ptr == NULL)
                return -1;
            *lineptr = new_ptr;
        }
        (*lineptr)[pos++] = c;
        if (c == '\n')
            break;
    }
    if (c == EOF && pos == 0)
        return -1;
    (*lineptr)[pos] = '\0';
    return pos;
}
