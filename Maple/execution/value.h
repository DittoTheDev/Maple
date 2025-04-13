#ifndef VALUE_H
#define VALUE_H

typedef enum {
    VAL_INT,
    VAL_STRING,
    VAL_FUNC,
    VAL_NONE
} ValueType;

typedef struct Value {
    ValueType type;
    union {
        int intValue;
        char* stringValue;
        struct {
            char* name;
            struct Value* (*fn)(struct Value** args, int argCount);
        } func;
    };
} Value;

Value* value_new_int(int i);
Value* value_new_string(const char* s);
Value* value_none();
void value_free(Value* val);

#endif
