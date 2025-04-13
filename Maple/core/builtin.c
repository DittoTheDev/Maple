#include "builtin.h"
#include <stdio.h>

void builtin_system_print(Value** args, int argCount) {
    for (int i = 0; i < argCount; i++) {
        if (args[i]->type == VAL_STRING)
            printf("%s", args[i]->stringValue);
        else if (args[i]->type == VAL_INT)
            printf("%d", args[i]->intValue);
    }
    printf("\n");
}

void register_builtins() {
}
