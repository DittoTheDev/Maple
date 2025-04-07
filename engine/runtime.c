#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MVM *mvm_create(ByteCode *bc) {
    MVM *vm = malloc(sizeof(MVM));
    vm->vars = malloc(sizeof(Variable) * 128);
    vm->var_count = 0;
    vm->sp = 0;
    vm->code = bc;
    return vm;
}

void mvm_run(MVM *vm) { // fixed 1.0.0 memory leak
    for (int i = 0; i < vm->code->count; ++i) {
        Instruction instr = vm->code->instructions[i];
        switch (instr.op) {
            case OP_LOAD_CONST:
                vm->stack[vm->sp++] = strdup(instr.arg);
                break;
                case OP_PRINT: {
                    char *val = vm->stack[--vm->sp];
                    printf("%s\n", val);
                    free(val);
                    break;
                }
            case OP_ADD: {
                char *b = vm->stack[--vm->sp];
                char *a = vm->stack[--vm->sp];
                char *res = malloc(strlen(a) + strlen(b) + 1);
                strcpy(res, a);
                strcat(res, b);
                vm->stack[vm->sp++] = res;
                free(a); free(b);
                break;
            }
            case OP_TO_STRING:
                break;
                case OP_END:
                while (vm->sp > 0) {
                    free(vm->stack[--vm->sp]); 
                }
                return;
            
            default:
                printf("Unknown opcode\n");
        }
    }
}

void mvm_destroy(MVM *vm) {
    for (int i = 0; i < vm->sp; ++i)
        free(vm->stack[i]);
    free(vm->vars);
    bytecode_free(vm->code);
    free(vm);
}
