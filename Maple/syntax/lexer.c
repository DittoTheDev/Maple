#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static char lexer_peek(Lexer* lexer) {
    return lexer->src[lexer->pos];
}

static char lexer_advance(Lexer* lexer) {
    char c = lexer->src[lexer->pos];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->pos++;
    return c;
}

static Token* token_new(TokenType type, const char* lexeme, int line, int column) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->lexeme = strdup(lexeme);
    token->line = line;
    token->column = column;
    return token;
}

Lexer* lexer_new(const char* src) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    lexer->src = src;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

void lexer_free(Lexer* lexer) {
    free(lexer);
}

Token* lexer_next_token(Lexer* lexer) {
    while (isspace(lexer_peek(lexer))) lexer_advance(lexer);
    int startPos = lexer->pos;
    int startLine = lexer->line;
    int startColumn = lexer->column;
    char c = lexer_peek(lexer);
    if (c == '\0')
        return token_new(TOKEN_EOF, "EOF", startLine, startColumn);
    if (isalpha(c) || c == '_') {
        while (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_')
            lexer_advance(lexer);
        int len = lexer->pos - startPos;
        char* lexeme = (char*)malloc(len+1);
        strncpy(lexeme, lexer->src + startPos, len);
        lexeme[len] = '\0';
        Token* token = token_new(TOKEN_IDENTIFIER, lexeme, startLine, startColumn);
        free(lexeme);
        return token;
    }
    if (isdigit(c)) {
        while (isdigit(lexer_peek(lexer))) lexer_advance(lexer);
        int len = lexer->pos - startPos;
        char* lexeme = (char*)malloc(len+1);
        strncpy(lexeme, lexer->src + startPos, len);
        lexeme[len] = '\0';
        Token* token = token_new(TOKEN_NUMBER, lexeme, startLine, startColumn);
        free(lexeme);
        return token;
    }
    if (c == '"') {
        lexer_advance(lexer);
        startPos = lexer->pos;
        while (lexer_peek(lexer) != '"' && lexer_peek(lexer) != '\0')
            lexer_advance(lexer);
        int len = lexer->pos - startPos;
        char* lexeme = (char*)malloc(len+1);
        strncpy(lexeme, lexer->src + startPos, len);
        lexeme[len] = '\0';
        if (lexer_peek(lexer) == '"')
            lexer_advance(lexer);
        Token* token = token_new(TOKEN_STRING, lexeme, startLine, startColumn);
        free(lexeme);
        return token;
    }
    lexer_advance(lexer);
    char op[2] = { c, '\0' };
    return token_new(TOKEN_OPERATOR, op, startLine, startColumn);
}
