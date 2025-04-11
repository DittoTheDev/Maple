#ifndef ENGINE_H
#define ENGINE_H

typedef struct MapleEngine MapleEngine;

// Create a new Maple engine instance.
MapleEngine* maple_create();

// Destroy the Maple engine.
void maple_destroy(MapleEngine *engine);

// Run the provided bytecode file (.mbc).
void maple_run(MapleEngine *engine, const char* filename);

#endif // ENGINE_H
