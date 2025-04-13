#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

typedef struct Pair {
    char *key;
    char *value;
    struct Pair *next;
} Pair;

struct Config {
    Pair* head;
};

Config* config_parse(const char* path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    Config* cfg = (Config*)malloc(sizeof(Config));
    cfg->head = NULL;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        char *p = line;
        if (p[0]=='#' || p[0]=='\n') continue;
        char key[128], value[256];
        if (sscanf(p, "%127s %255s", key, value) != 2) continue;
        Pair* pair = (Pair*)malloc(sizeof(Pair));
        pair->key = strdup(key);
        pair->value = strdup(value);
        pair->next = cfg->head;
        cfg->head = pair;
    }
    fclose(f);
    return cfg;
}

char* config_get(Config* cfg, const char* key) {
    for(Pair* p = cfg->head; p; p = p->next) {
        if(strcmp(p->key, key)==0)
            return strdup(p->value);
    }
    return NULL;
}

void config_free(Config* cfg) {
    Pair* p = cfg->head;
    while(p) {
        Pair* next = p->next;
        free(p->key);
        free(p->value);
        free(p);
        p = next;
    }
    free(cfg);
}
