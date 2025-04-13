#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "value.h"

typedef struct Symbol {
    char* name;
    Value* value;
    struct Symbol* next;
} Symbol;

typedef struct Interpreter {
    Symbol* symbols;
} Interpreter;

Interpreter* interpreter_new();
void interpreter_free(Interpreter* interp);
Value* interpreter_eval(Interpreter* interp, ASTNode* node);
void interpreter_set_variable(Interpreter* interp, const char* name, Value* value);
Value* interpreter_get_variable(Interpreter* interp, const char* name);

#endif
