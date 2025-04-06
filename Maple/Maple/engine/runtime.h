#ifndef MAPLE_RUNTIME_H
#define MAPLE_RUNTIME_H

#include "bytecode.h"

typedef struct {
    char *key;
    char *value;
} Variable;

typedef struct {
    Variable *vars;
    int var_count;
    char *stack[256];
    int sp;
    ByteCode *code;
} MVM;

MVM *mvm_create(ByteCode *bc);
void mvm_run(MVM *vm);
void mvm_destroy(MVM *vm);

#endif
