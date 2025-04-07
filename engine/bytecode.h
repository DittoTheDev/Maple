#ifndef MAPLE_BYTECODE_H
#define MAPLE_BYTECODE_H

typedef enum {
    OP_LOAD_CONST,
    OP_STORE_VAR,
    OP_LOAD_VAR,
    OP_ADD,
    OP_SUB,
    OP_PRINT,
    OP_TO_STRING,
    OP_CALL,
    OP_RETURN,
    OP_LET,       // New: let(var, value)
    OP_CALC,      // New: calc(expression)
    OP_IF,        // New: if(condition){ ... }
    OP_ENDIF,     // New: end of if block
    OP_LOOP,      // New: loop(start, end){ ... }
    OP_ENDLOOP,   // New: end of loop block
    OP_IMPORT,    // New: import(module)
    OP_END
} OpCode;

typedef struct {
    OpCode op;
    char *arg;
} Instruction;

typedef struct {
    Instruction *instructions;
    int count;
    int capacity;
} ByteCode;

ByteCode *bytecode_load_from_file(const char *filename);
void bytecode_free(ByteCode *bc);

#endif
