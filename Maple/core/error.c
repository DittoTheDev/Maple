#include "error.h"
#include <stdio.h>
#include <stdarg.h>

void maple_error(Loc loc, const char *format, ...) {
    fprintf(stderr, "Error [%s:%d %s]: ", loc.file, loc.line, loc.func);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void maple_runtime_error(const char *format, ...) {
    fprintf(stderr, "Runtime Error: ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}
