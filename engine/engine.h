#ifndef ENGINE_H
#define ENGINE_H

typedef struct MapleEngine MapleEngine;

// Create a new Maple engine instance.
MapleEngine* maple_create();

// Destroy and clean up the engine.
void maple_destroy(MapleEngine *engine);

// Run the given bytecode file.
void maple_run(MapleEngine *engine, const char* filename);

#endif // ENGINE_H
