#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

VM* vm_new(BytecodeChunk* chunk) {
    VM* vm = (VM*)malloc(sizeof(VM));
    vm->chunk = chunk;
    vm->ip = 0;
    return vm;
}

void vm_free(VM* vm) {
    free(vm);
}

Value* vm_run(VM* vm) {
    while (vm->ip < vm->chunk->count) {
        Bytecode inst = vm->chunk->code[vm->ip++];
        switch (inst) {
            case BC_NOP: break;
            case BC_PUSH_INT: printf("VM: PUSH_INT (dummy 123)\n"); break;
            case BC_PUSH_STRING: printf("VM: PUSH_STRING (dummy \"Hello\")\n"); break;
            case BC_PRINT: printf("VM: PRINT (dummy output 123)\n"); break;
            default: printf("VM: Unknown opcode %d\n", inst); break;
        }
    }
    return NULL;
}
