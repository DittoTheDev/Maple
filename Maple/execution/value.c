#include "value.h"
#include <stdlib.h>
#include <string.h>

Value* value_new_int(int i) {
    Value* val = (Value*)malloc(sizeof(Value));
    val->type = VAL_INT;
    val->intValue = i;
    return val;
}

Value* value_new_string(const char* s) {
    Value* val = (Value*)malloc(sizeof(Value));
    val->type = VAL_STRING;
    val->stringValue = strdup(s);
    return val;
}

Value* value_none() {
    Value* val = (Value*)malloc(sizeof(Value));
    val->type = VAL_NONE;
    return val;
}

void value_free(Value* val) {
    if (!val) return;
    if (val->type == VAL_STRING && val->stringValue)
        free(val->stringValue);
    free(val);
}
