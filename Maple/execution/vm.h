#ifndef VM_H
#define VM_H

#include "compiler.h"
#include "value.h"

typedef struct {
    BytecodeChunk* chunk;
    int ip;
} VM;

VM* vm_new(BytecodeChunk* chunk);
void vm_free(VM* vm);
Value* vm_run(VM* vm);

#endif
