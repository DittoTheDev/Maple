#ifndef COMPILER_H
#define COMPILER_H

#include "ast.h"

typedef enum {
    BC_NOP,
    BC_PUSH_INT,
    BC_PUSH_STRING,
    BC_PRINT
} Bytecode;

typedef struct {
    Bytecode* code;
    int count;
    int capacity;
} BytecodeChunk;

typedef struct {
    BytecodeChunk* chunk;
} Compiler;

Compiler* compiler_new();
void compiler_free(Compiler* compiler);
int compile_ast(Compiler* compiler, ASTNode* node);
void dump_bytecode(Compiler* compiler, const char* filename);

#endif
