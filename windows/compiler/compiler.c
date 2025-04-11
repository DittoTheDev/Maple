#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler/compiler.h"

// Define our opcode constants.
#define OP_PRINT       0x01
#define OP_OS_PWD      0x02
#define OP_OS_LISTDIR  0x03
#define OP_OS_SYSTEM   0x04
#define OP_IMPORT      0x05
#define OP_EXIT        0x06

// Helper: Trim leading and trailing whitespace.
static char* trim_whitespace(char* str) {
    char* end;
    // Trim leading spaces.
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)
        return str;
    // Trim trailing spaces.
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

int maple_compile(const char* source_filename, const char* output_filename) {
    FILE* src = fopen(source_filename, "r");
    if (!src) {
        fprintf(stderr, "Error: Could not open source file: %s\n", source_filename);
        return 1;
    }

    FILE* out = fopen(output_filename, "wb");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file: %s\n", output_filename);
        fclose(src);
        return 1;
    }

    char line[1024];
    int line_number = 0;
    int error_occurred = 0;
    while (fgets(line, sizeof(line), src)) {
        line_number++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        char* trimmed = trim_whitespace(line);
        if (strlen(trimmed) == 0)
            continue; // Skip empty lines.

        // Process each command.
        if (strncmp(trimmed, "print", 5) == 0) {
            // Format: print <text>
            char* p = trimmed + 5;
            p = trim_whitespace(p);
            unsigned char opcode = OP_PRINT;
            fwrite(&opcode, sizeof(opcode), 1, out);
            fwrite(p, sizeof(char), strlen(p) + 1, out);
        }
        else if (strcmp(trimmed, "os.pwd()") == 0) {
            unsigned char opcode = OP_OS_PWD;
            fwrite(&opcode, sizeof(opcode), 1, out);
        }
        else if (strncmp(trimmed, "os.listdir(", 11) == 0) {
            // Format: os.listdir(<dir>)
            char* start = strchr(trimmed, '(');
            char* end = strrchr(trimmed, ')');
            if (!start || !end || start >= end) {
                fprintf(stderr, "Error at line %d: Invalid os.listdir syntax.\n", line_number);
                error_occurred = 1; break;
            }
            *end = '\0';
            char* param = start + 1;
            param = trim_whitespace(param);
            unsigned char opcode = OP_OS_LISTDIR;
            fwrite(&opcode, sizeof(opcode), 1, out);
            fwrite(param, sizeof(char), strlen(param) + 1, out);
        }
        else if (strncmp(trimmed, "os.system(", 10) == 0) {
            // Format: os.system(<cmd>)
            char* start = strchr(trimmed, '(');
            char* end = strrchr(trimmed, ')');
            if (!start || !end || start >= end) {
                fprintf(stderr, "Error at line %d: Invalid os.system syntax.\n", line_number);
                error_occurred = 1; break;
            }
            *end = '\0';
            char* param = start + 1;
            param = trim_whitespace(param);
            unsigned char opcode = OP_OS_SYSTEM;
            fwrite(&opcode, sizeof(opcode), 1, out);
            fwrite(param, sizeof(char), strlen(param) + 1, out);
        }
        else if (strncmp(trimmed, "import", 6) == 0) {
            // Format: import <module>
            char* p = trimmed + 6;
            p = trim_whitespace(p);
            if (strlen(p) == 0) {
                fprintf(stderr, "Error at line %d: Invalid import syntax.\n", line_number);
                error_occurred = 1; break;
            }
            unsigned char opcode = OP_IMPORT;
            fwrite(&opcode, sizeof(opcode), 1, out);
            fwrite(p, sizeof(char), strlen(p) + 1, out);
        }
        else if (strcmp(trimmed, "exit") == 0) {
            unsigned char opcode = OP_EXIT;
            fwrite(&opcode, sizeof(opcode), 1, out);
        }
        else {
            fprintf(stderr, "Error at line %d: Unknown command: %s\n", line_number, trimmed);
            error_occurred = 1; break;
        }
    }
    fclose(src);
    fclose(out);

    if (error_occurred) {
        remove(output_filename);  // Delete invalid output.
        return 1;
    }
    return 0;
}
