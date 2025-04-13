#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

typedef struct {
    Lexer* lexer;
    Token* current;
} Parser;

Parser* parser_new(const char* src);
void parser_free(Parser* parser);
ASTNode* parse_program(Parser* parser);

#endif
