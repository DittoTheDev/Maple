#include "bytecode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ByteCode *bytecode_load_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;
    ByteCode *bc = malloc(sizeof(ByteCode));
    bc->instructions = malloc(sizeof(Instruction) * 256);
    bc->count = 0;
    bc->capacity = 256;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        Instruction instr;
        char op[64] = {0}, arg[128] = {0};
        int matched = sscanf(line, "%s %[^\n]", op, arg);
        if (matched == 1) {
            arg[0] = '\0';
        }
        if (strcmp(op, "LOAD_CONST") == 0)
            instr.op = OP_LOAD_CONST;
        else if (strcmp(op, "STORE_VAR") == 0)
            instr.op = OP_STORE_VAR;
        else if (strcmp(op, "LOAD_VAR") == 0)
            instr.op = OP_LOAD_VAR;
        else if (strcmp(op, "ADD") == 0)
            instr.op = OP_ADD;
        else if (strcmp(op, "SUB") == 0)
            instr.op = OP_SUB;
        else if (strcmp(op, "PRINT") == 0)
            instr.op = OP_PRINT;
        else if (strcmp(op, "TO_STRING") == 0)
            instr.op = OP_TO_STRING;
        else if (strcmp(op, "CALL") == 0)
            instr.op = OP_CALL;
        else if (strcmp(op, "RETURN") == 0)
            instr.op = OP_RETURN;
        else if (strcmp(op, "LET") == 0)
            instr.op = OP_LET;
        else if (strcmp(op, "CALC") == 0)
            instr.op = OP_CALC;
        else if (strcmp(op, "IF") == 0)
            instr.op = OP_IF;
        else if (strcmp(op, "ENDIF") == 0)
            instr.op = OP_ENDIF;
        else if (strcmp(op, "LOOP") == 0)
            instr.op = OP_LOOP;
        else if (strcmp(op, "ENDLOOP") == 0)
            instr.op = OP_ENDLOOP;
        else if (strcmp(op, "IMPORT") == 0)
            instr.op = OP_IMPORT;
        else if (strcmp(op, "END") == 0)
            instr.op = OP_END;
        else
            instr.op = OP_END; // Fallback

        instr.arg = strdup(arg);
        bc->instructions[bc->count++] = instr;
    }
    fclose(fp);
    return bc;
}

void bytecode_free(ByteCode *bc) {
    for (int i = 0; i < bc->count; ++i)
        free(bc->instructions[i].arg);
    free(bc->instructions);
    free(bc);
}
