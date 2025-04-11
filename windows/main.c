#include <stdio.h>
#include <string.h>
#include "engine/engine.h"

#define MAPLE_VERSION "1.1"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: mvm <filename.mbc> | --version\n");
        return 1;
    }

    if ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "-v") == 0)) {
        printf("Maple version %s\n", MAPLE_VERSION);
        return 0;
    }

    MapleEngine *engine = maple_create();
    if (engine == NULL) {
        fprintf(stderr, "Error: Unable to initialize Maple engine.\n");
        return 1;
    }

    maple_run(engine, argv[1]);
    maple_destroy(engine);

    return 0;
}
