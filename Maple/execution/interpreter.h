#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "value.h"

typedef struct {
} Interpreter;

Interpreter* interpreter_new();
void interpreter_free(Interpreter* interp);
Value* interpreter_eval(Interpreter* interp, ASTNode* node);

#endif
