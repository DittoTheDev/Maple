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
    FILE *in = fopen(input_file, "r"); // ✅ Move this up first
    if (!in) {
        printf("Error: Could not open %s\n", input_file);
        return;
    }

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

        // ✅ Skip comments and shebang
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
    }

    fprintf(out, "END\n");

    fclose(in);
    fclose(out);

    printf("Compiled %s -> %s\n", input_file, output_path);
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: maple <file.maple>\n");
        return 1;
    }

    ensure_cache_dir();

    for (int i = 1; i < argc; i++) {
        compile(argv[i]);
    }

    return 0;
}

