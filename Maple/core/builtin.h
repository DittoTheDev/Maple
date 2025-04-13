#ifndef BUILTIN_H
#define BUILTIN_H

#include "value.h"

void builtin_system_print(Value** args, int argCount);
void register_builtins();

#endif
