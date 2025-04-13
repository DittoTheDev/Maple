#ifndef ERROR_H
#define ERROR_H

typedef struct { const char *file; int line; const char *func; } Loc;
void maple_error(Loc loc, const char *format, ...);
void maple_runtime_error(const char *format, ...);

#endif
