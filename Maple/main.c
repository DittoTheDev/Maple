#include <stdio.h>
#include "engine/engine.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./mvm <filename.mbc>\n");
        return 1;
    }

    MapleEngine *engine = maple_create();
    maple_run(engine, argv[1]);
    maple_destroy(engine);
    return 0;
}
