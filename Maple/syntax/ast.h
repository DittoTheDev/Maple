#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_USE,
    AST_FUNC_DECL,
    AST_BLOCK,
    AST_EXPR_STATEMENT,
    AST_CALL_EXPR,
    AST_BINARY_EXPR,
    AST_LITERAL,
    AST_IDENTIFIER,
    AST_VAR_DECL,      // New: variable declaration (e.g., "var x = expr")
    AST_ASSIGNMENT     // New: assignment (e.g., "x = expr")
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char *value;  // For identifiers, operator symbols, or variable names
    struct ASTNode **children;
    int child_count;
} ASTNode;

ASTNode* ast_new_node(ASTNodeType type, const char* value);
void ast_add_child(ASTNode* parent, ASTNode* child);
void ast_free(ASTNode* node);

#endif
