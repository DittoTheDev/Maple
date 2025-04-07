#include "engine.h"
#include "runtime.h"
#include "bytecode.h"
#include <stdlib.h>
#include <stdio.h>

MapleEngine *maple_create() {
    MapleEngine *engine = malloc(sizeof(MapleEngine));
    engine->vm = NULL;
    return engine;
}

void maple_run(MapleEngine *engine, const char *filename) {
    ByteCode *bc = bytecode_load_from_file(filename);
    if (!bc) {
        printf("Could not load bytecode from file: %s\n", filename);
        return;
    }
    engine->vm = mvm_create(bc);
    mvm_run(engine->vm);
}

void maple_destroy(MapleEngine *engine) {
    if (engine->vm) {
        mvm_destroy(engine->vm);
    }
    free(engine);
}
