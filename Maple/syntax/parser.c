#include "parser.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Forward declaration
static ASTNode* parse_statement(Parser* parser);

static void parser_advance(Parser* parser) {
    free(parser->current->lexeme);
    free(parser->current);
    parser->current = lexer_next_token(parser->lexer);
}

Parser* parser_new(const char* src) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = lexer_new(src);
    parser->current = lexer_next_token(parser->lexer);
    return parser;
}

void parser_free(Parser* parser) {
    free(parser->current);
    lexer_free(parser->lexer);
    free(parser);
}

static ASTNode* parse_use(Parser* parser) {
    ASTNode* node = ast_new_node(AST_USE, NULL);
    parser_advance(parser);  // skip 'use'
    if (parser->current->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected module name after 'use'\n");
        return NULL;
    }
    ASTNode* mod = ast_new_node(AST_LITERAL, parser->current->lexeme);
    ast_add_child(node, mod);
    parser_advance(parser);
    return node;
}

static ASTNode* parse_func_decl(Parser* parser) {
    char* retType = strdup(parser->current->lexeme);
    parser_advance(parser);
    if (parser->current->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected function name after type\n");
        free(retType);
        return NULL;
    }
    char* funcName = strdup(parser->current->lexeme);
    parser_advance(parser);
    if (strcmp(parser->current->lexeme, "{") != 0) {
        fprintf(stderr, "Expected '{' after function name\n");
        free(retType); 
        free(funcName);
        return NULL;
    }
    parser_advance(parser);
    ASTNode* block = ast_new_node(AST_BLOCK, NULL);
    while (parser->current && strcmp(parser->current->lexeme, "}") != 0 &&
           parser->current->type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt)
            ast_add_child(block, stmt);
    }
    if (parser->current && strcmp(parser->current->lexeme, "}") == 0)
        parser_advance(parser);
    ASTNode* funcDecl = ast_new_node(AST_FUNC_DECL, funcName);
    ast_add_child(funcDecl, block);
    free(retType); 
    free(funcName);
    return funcDecl;
}

static ASTNode* parse_var_decl(Parser* parser) {
    // current token is "var"
    parser_advance(parser); // skip "var"
    if (parser->current->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected variable name after 'var'\n");
        return NULL;
    }
    char* varName = strdup(parser->current->lexeme);
    parser_advance(parser); // skip variable name
    if (strcmp(parser->current->lexeme, "=") != 0) {
        fprintf(stderr, "Expected '=' after variable name\n");
        free(varName);
        return NULL;
    }
    parser_advance(parser); // skip '='
    // Parse the initialization expression.
    ASTNode* expr = parse_statement(parser);
    ASTNode* node = ast_new_node(AST_VAR_DECL, varName);
    ast_add_child(node, expr);
    free(varName);
    return node;
}

static ASTNode* parse_assignment(Parser* parser, ASTNode* left) {
    // At this point, 'left' is an identifier
    if (strcmp(parser->current->lexeme, "=") != 0)
        return left;  // no assignment operator, simply return the identifier node
    parser_advance(parser); // skip '=' token
    ASTNode* expr = parse_statement(parser);
    ASTNode* node = ast_new_node(AST_ASSIGNMENT, left->value);
    ast_add_child(node, expr);
    return node;
}

static ASTNode* parse_statement(Parser* parser) {
    if (parser->current->type == TOKEN_IDENTIFIER) {
        if (strcmp(parser->current->lexeme, "use") == 0)
            return parse_use(parser);
        if (strcmp(parser->current->lexeme, "void") == 0)
            return parse_func_decl(parser);
        if (strcmp(parser->current->lexeme, "var") == 0)
            return parse_var_decl(parser);
        // Handle assignment: if an identifier is followed by '='
        ASTNode* identifierNode = ast_new_node(AST_IDENTIFIER, parser->current->lexeme);
        parser_advance(parser);
        if (strcmp(parser->current->lexeme, "=") == 0)
            return parse_assignment(parser, identifierNode);
        return identifierNode;
    }
    // Otherwise, treat as a literal expression.
    ASTNode* expr = ast_new_node(AST_LITERAL, parser->current->lexeme);
    parser_advance(parser);
    return expr;
}

ASTNode* parse_program(Parser* parser) {
    ASTNode* program = ast_new_node(AST_PROGRAM, NULL);
    while (parser->current && parser->current->type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt)
            ast_add_child(program, stmt);
    }
    return program;
}
