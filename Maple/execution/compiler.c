#include "compiler.h"
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void chunk_init(BytecodeChunk* chunk) {
    chunk->capacity = 64;
    chunk->code = (Bytecode*)malloc(sizeof(Bytecode) * chunk->capacity);
    chunk->count = 0;
}

static void chunk_write(BytecodeChunk* chunk, Bytecode byte) {
    if (chunk->count >= chunk->capacity) {
        chunk->capacity *= 2;
        chunk->code = (Bytecode*)realloc(chunk->code, sizeof(Bytecode) * chunk->capacity);
    }
    chunk->code[chunk->count++] = byte;
}

Compiler* compiler_new() {
    Compiler* compiler = (Compiler*)malloc(sizeof(Compiler));
    compiler->chunk = (BytecodeChunk*)malloc(sizeof(BytecodeChunk));
    chunk_init(compiler->chunk);
    return compiler;
}

void compiler_free(Compiler* compiler) {
    free(compiler->chunk->code);
    free(compiler->chunk);
    free(compiler);
}

static int compile_node(Compiler* compiler, ASTNode* node) {
    if (node->type == AST_LITERAL) {
        int isNum = (node->value && strspn(node->value, "0123456789") == strlen(node->value));
        chunk_write(compiler->chunk, isNum ? BC_PUSH_INT : BC_PUSH_STRING);
        return 0;
    }
    if (node->type == AST_EXPR_STATEMENT) {
        if (node->child_count > 0)
            compile_node(compiler, node->children[0]);
        chunk_write(compiler->chunk, BC_PRINT);
        return 0;
    }
    return 0;
}

int compile_ast(Compiler* compiler, ASTNode* node) {
    compile_node(compiler, node);
    return 0;
}

void dump_bytecode(Compiler* compiler, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) { perror("Cannot open bytecode file"); return; }
    fwrite(compiler->chunk->code, sizeof(Bytecode), compiler->chunk->count, f);
    fclose(f);
    printf("Bytecode written to %s\n", filename);
}
