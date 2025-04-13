#include "interpreter.h"
#include "ast.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Interpreter* interpreter_new() {
    Interpreter* interp = (Interpreter*)malloc(sizeof(Interpreter));
    return interp;
}

void interpreter_free(Interpreter* interp) {
    free(interp);
}

static Value* eval_program(Interpreter* interp, ASTNode* node) {
    Value* result = value_none();
    for (int i = 0; i < node->child_count; i++)
        result = interpreter_eval(interp, node->children[i]);
    return result;
}

static Value* eval_literal(Interpreter* interp, ASTNode* node) {
    if (node->value) {
        int isNum = (strspn(node->value, "0123456789") == strlen(node->value));
        if (isNum)
            return value_new_int(atoi(node->value));
        else
            return value_new_string(node->value);
    }
    return value_none();
}

static Value* eval_expr_statement(Interpreter* interp, ASTNode* node) {
    if (node->child_count > 0)
        return interpreter_eval(interp, node->children[0]);
    return value_none();
}

Value* interpreter_eval(Interpreter* interp, ASTNode* node) {
    switch (node->type) {
        case AST_PROGRAM: return eval_program(interp, node);
        case AST_EXPR_STATEMENT: return eval_expr_statement(interp, node);
        case AST_LITERAL: return eval_literal(interp, node);
        default:
            printf("Unknown AST node type: %d\n", node->type);
            return value_none();
    }
}
