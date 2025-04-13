#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

typedef struct Config Config;
Config* config_parse(const char* path);
char* config_get(Config* cfg, const char* key);
void config_free(Config* cfg);

#endif
