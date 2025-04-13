#include "interpreter.h"
#include "ast.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef long ssize_t; was not working so it is removed
// Helper functions for symbol table management.
void interpreter_set_variable(Interpreter* interp, const char* name, Value* value) {
    Symbol* cur = interp->symbols;
    while(cur) {
        if (strcmp(cur->name, name) == 0) {
            value_free(cur->value);
            cur->value = value;
            return;
        }
        cur = cur->next;
    }
    Symbol* newSym = (Symbol*)malloc(sizeof(Symbol));
    newSym->name = strdup(name);
    newSym->value = value;
    newSym->next = interp->symbols;
    interp->symbols = newSym;
}

Value* interpreter_get_variable(Interpreter* interp, const char* name) {
    Symbol* cur = interp->symbols;
    while(cur) {
        if (strcmp(cur->name, name) == 0)
            return cur->value;
        cur = cur->next;
    }
    return value_none(); // Return a default value if not found.
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
        case AST_PROGRAM:
            return eval_program(interp, node);
        case AST_EXPR_STATEMENT:
            return eval_expr_statement(interp, node);
        case AST_LITERAL:
            return eval_literal(interp, node);
        case AST_IDENTIFIER:
            return interpreter_get_variable(interp, node->value);
        case AST_VAR_DECL: {
            // Evaluate initializer and store in symbol table.
            Value* val = interpreter_eval(interp, node->children[0]);
            interpreter_set_variable(interp, node->value, val);
            return val;
        }
        case AST_ASSIGNMENT: {
            // Evaluate the new value and update the variable.
            Value* val = interpreter_eval(interp, node->children[0]);
            interpreter_set_variable(interp, node->value, val);
            return val;
        }
        case AST_BINARY_EXPR: {
            // For simplicity, assume binary operator is stored in node->value
            // and there are exactly two children.
            Value* left = interpreter_eval(interp, node->children[0]);
            Value* right = interpreter_eval(interp, node->children[1]);
            if (strcmp(node->value, "+") == 0) {
                if (left->type == VAL_INT && right->type == VAL_INT)
                    return value_new_int(left->intValue + right->intValue);
                if (left->type == VAL_STRING && right->type == VAL_STRING) {
                    size_t len = strlen(left->stringValue) + strlen(right->stringValue) + 1;
                    char* res = malloc(len);
                    strcpy(res, left->stringValue);
                    strcat(res, right->stringValue);
                    return value_new_string(res);
                }
            }
            return value_none();
        }
        default:
            printf("Unknown AST node type: %d\n", node->type);
            return value_none();
    }
}

Interpreter* interpreter_new() {
    Interpreter* interp = (Interpreter*)malloc(sizeof(Interpreter));
    interp->symbols = NULL;
    return interp;
}

void interpreter_free(Interpreter* interp) {
    Symbol* cur = interp->symbols;
    while(cur) {
        Symbol* next = cur->next;
        free(cur->name);
        value_free(cur->value);
        free(cur);
        cur = next;
    }
    free(interp);
}
