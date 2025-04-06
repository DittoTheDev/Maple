#ifndef MAPLE_ENGINE_H
#define MAPLE_ENGINE_H

#include "bytecode.h"
#include "runtime.h"

typedef struct {
    MVM *vm;
} MapleEngine;

MapleEngine *maple_create();
void maple_run(MapleEngine *engine, const char *filename);
void maple_destroy(MapleEngine *engine);

#endif
