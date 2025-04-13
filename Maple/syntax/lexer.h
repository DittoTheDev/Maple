#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    const char *src;
    int pos;
    int line;
    int column;
} Lexer;

Lexer* lexer_new(const char* src);
void lexer_free(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);

#endif
