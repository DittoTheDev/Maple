#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode* ast_new_node(ASTNodeType type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->child_count = 0;
    node->children = NULL;
    node->value = value ? strdup(value) : NULL;
    return node;
}

void ast_add_child(ASTNode* parent, ASTNode* child) {
    parent->child_count++;
    parent->children = (ASTNode**)realloc(parent->children,
                                          parent->child_count * sizeof(ASTNode*));
    parent->children[parent->child_count - 1] = child;
}

void ast_free(ASTNode* node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++)
        ast_free(node->children[i]);
    free(node->children);
    if (node->value) free(node->value);
    free(node);
}
