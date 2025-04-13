#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"
#include "shell.h"
#include "config_parser.h"
#include "error.h"
#include "parser.h"
#include "interpreter.h"
#include "ast.h"
#include "compiler.h"
#include "vm.h"
#include "logger.h"

int main(int argc, char** argv) {
    runtime_init();

    if (argc < 2) {
        printf("Maple REPL\n");
        shell();
    } else {
        if (strcmp(argv[1], "-c") == 0 && argc >= 5) {
            const char* srcFile = argv[2];
            const char* outFile = argv[4];
            FILE* f = fopen(srcFile, "r");
            if (!f) {
                maple_error((Loc){__FILE__,__LINE__,"main"}, "Cannot open source file: %s", srcFile);
                exit(1);
            }
            fseek(f, 0, SEEK_END);
            long len = ftell(f);
            fseek(f, 0, SEEK_SET);
            char* source = malloc(len+1);
            fread(source, 1, len, f);
            source[len] = '\0';
            fclose(f);

            Parser* parser = parser_new(source);
            ASTNode* ast = parse_program(parser);
            Compiler* compiler = compiler_new();
            compile_ast(compiler, ast);
            dump_bytecode(compiler, outFile);

            VM* vm = vm_new(compiler->chunk);
            vm_run(vm);
            vm_free(vm);
            compiler_free(compiler);
            ast_free(ast);
            parser_free(parser);
            free(source);
        } else if (strcmp(argv[1], "-r") == 0 && argc >= 3) {
            const char* srcFile = argv[2];
            FILE* f = fopen(srcFile, "r");
            if (!f) {
                maple_error((Loc){__FILE__,__LINE__,"main"}, "Cannot open source file: %s", srcFile);
                exit(1);
            }
            fseek(f, 0, SEEK_END);
            long len = ftell(f);
            fseek(f, 0, SEEK_SET);
            char* source = malloc(len+1);
            fread(source, 1, len, f);
            source[len] = '\0';
            fclose(f);

            Parser* parser = parser_new(source);
            ASTNode* ast = parse_program(parser);
            Interpreter* interp = interpreter_new();
            interpreter_eval(interp, ast);
            ast_free(ast);
            parser_free(parser);
            interpreter_free(interp);
            free(source);
        } else {
            printf("Usage:\n  %s -c <source> -o <output.bytecode>\n  %s -r <source>\n", argv[0], argv[0]);
        }
    }
    
    runtime_shutdown();
    return 0;
}
