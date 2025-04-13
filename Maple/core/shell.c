#include "shell.h"
#include "parser.h"
#include "interpreter.h"
#include "ast.h"
#include "value.h"
#include "error.h"
#include "getline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_help() {
    printf("Maple Shell Commands:\n");
    printf("  .help            Display help\n");
    printf("  .exit            Exit shell\n");
    printf("  .version         Show version\n");
    printf("  .compile <src> -o <out>  Compile source file\n");
    printf("  .run <file>      Run source file\n");
}

void shell() {
    char *line = NULL;
    size_t len = 0;
    Interpreter* interp = interpreter_new();
    printf("Maple REPL (.exit to quit)\n");
    while (1) {
        printf("maple> ");
        if (getline(&line, &len, stdin) < 0)
            break;
        if (strncmp(line, ".exit", 5) == 0)
            break;
        Parser* parser = parser_new(line);
        ASTNode* program = parse_program(parser);
        Value* result = interpreter_eval(interp, program);
        if (result) {
            if (result->type == VAL_INT)
                printf("=> %d\n", result->intValue);
            else if (result->type == VAL_STRING)
                printf("=> %s\n", result->stringValue);
            value_free(result);
        }
        ast_free(program);
        parser_free(parser);
    }
    free(line);
    interpreter_free(interp);
}
