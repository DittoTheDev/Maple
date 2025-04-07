#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir_dir(name) _mkdir(name)
#else
#include <unistd.h>
#define mkdir_dir(name) mkdir(name, 0755)
#endif

#define CACHE_DIR "--maplecache--"

void ensure_cache_dir() {
    struct stat st = {0};
    if (stat(CACHE_DIR, &st) == -1) {
        mkdir_dir(CACHE_DIR);
    }
}

void compile(const char *input_file) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        printf("Error: Could not open %s\n", input_file);
        return;
    }

    // Prepare output file path based on input filename.
    char output_path[256];
    const char *base = strrchr(input_file, '/');
    base = base ? base + 1 : input_file;
    char filename[128];
    strncpy(filename, base, sizeof(filename));
    char *dot = strrchr(filename, '.');
    if (dot) *dot = '\0';
    snprintf(output_path, sizeof(output_path), "%s/%s.mbc", CACHE_DIR, filename);

    FILE *out = fopen(output_path, "w");
    if (!out) {
        printf("Error: Cannot write to %s\n", output_path);
        fclose(in);
        return;
    }

    char line[256];
    int line_num = 0;
    while (fgets(line, sizeof(line), in)) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';

        // Skip shebang and comment lines.
        if (strncmp(line, "#!", 2) == 0 || strncmp(line, "//", 2) == 0) {
            continue;
        }
        if (strncmp(line, "print(", 6) == 0) {
            char *start = strchr(line, '"');
            char *end = strrchr(line, '"');
            if (start && end && end > start) {
                char buffer[200];
                strncpy(buffer, start + 1, end - start - 1);
                buffer[end - start - 1] = '\0';
                fprintf(out, "LOAD_CONST %s\n", buffer);
                fprintf(out, "PRINT\n");
            }
        }
        else if (strncmp(line, "let(", 4) == 0) {
            char var[64], value[128];
            if (sscanf(line, "let(%[^,], %[^\n)]", var, value) == 2) {
                fprintf(out, "LET %s %s\n", var, value);
            }
        }
        else if (strncmp(line, "calc(", 5) == 0) {
            char expr[128];
            char *start = strchr(line, '(');
            char *end = strrchr(line, ')');
            if (start && end && end > start) {
                int len = end - start - 1;
                strncpy(expr, start + 1, len);
                expr[len] = '\0';
                fprintf(out, "CALC %s\n", expr);
            }
        }
        else if (strncmp(line, "if(", 3) == 0 && strstr(line, "){") != NULL) {
            char cond[128];
            char *start = strchr(line, '(');
            char *end = strchr(line, ')');
            if (start && end && end > start) {
                int len = end - start - 1;
                strncpy(cond, start + 1, len);
                cond[len] = '\0';
                fprintf(out, "IF %s\n", cond);
            }
        }
        else if (strcmp(line, "}") == 0) {
            // We assume a '}' here ends an if-block.
            fprintf(out, "ENDIF\n");
        }
        else if (strncmp(line, "loop(", 5) == 0 && strstr(line, "){") != NULL) {
            char start_val[64], end_val[64];
            if (sscanf(line, "loop(%[^,], %[^)]", start_val, end_val) == 2) {
                fprintf(out, "LOOP %s %s\n", start_val, end_val);
            }
        }
        else if (strcmp(line, "}") == 0) {
            fprintf(out, "ENDLOOP\n");
        }
        else if (strncmp(line, "import(", 7) == 0) {
            char module[64];
            char *start = strchr(line, '(');
            char *end = strrchr(line, ')');
            if (start && end && end > start) {
                int len = end - start - 1;
                strncpy(module, start + 1, len);
                module[len] = '\0';
                fprintf(out, "IMPORT %s\n", module);
            }
        }
        else {
            printf("Warning: Unrecognized command at line %d: %s\n", line_num, line);
        }
    }

    fprintf(out, "END\n");

    fclose(in);
    fclose(out);

    printf("Compiled %s -> %s\n", input_file, output_path);

    char run_cmd[512];
    snprintf(run_cmd, sizeof(run_cmd), "mvm %s", output_path);
    system(run_cmd);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: maple <file.maple>\n");
        return 1;
    }

    if (strcmp(argv[1], "--version") == 0) {
        printf("Maple Compiler v1.0.1 Beta\n");
        return 0;
    }

    ensure_cache_dir();

    for (int i = 1; i < argc; i++) {
        compile(argv[i]);
    }

    return 0;
}
