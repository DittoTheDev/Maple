#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>     // For getcwd()
#include <dirent.h>     // For directory listing
#include "engine/engine.h"

// ======================
// MapleEngine Structure
// ======================
struct MapleEngine {
    // Engine state, context, symbol tables, etc. would be here.
};

// ================================
// Engine Creation and Destruction
// ================================
MapleEngine* maple_create() {
    MapleEngine *engine = (MapleEngine*) malloc(sizeof(MapleEngine));
    // Initialize engine state here if needed.
    return engine;
}

void maple_destroy(MapleEngine *engine) {
    if (engine) {
        free(engine);
    }
}

// ========================================
// Utility: Calculate String Similarity
// ========================================
// (Counts matching characters from the start for suggestions)
int similarity(const char *a, const char *b) {
    int count = 0;
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    size_t min_len = (len_a < len_b) ? len_a : len_b;
    for (size_t i = 0; i < min_len; i++) {
        if (a[i] == b[i]) {
            count++;
        }
    }
    return count;
}

// ==============================================
// Suggestion Mechanism: Check for Similar Commands
// ==============================================
const char* suggest_command(const char *token) {
    const char *candidates[] = {"print", "os", "import", "exit"};
    int candidateCount = sizeof(candidates) / sizeof(candidates[0]);
    int bestScore = 0;
    const char *bestCandidate = NULL;
    
    for (int i = 0; i < candidateCount; i++) {
        int score = similarity(token, candidates[i]);
        if (score > bestScore) {
            bestScore = score;
            bestCandidate = candidates[i];
        }
    }
    // Offer a suggestion if at least 2 characters match.
    return (bestScore >= 2) ? bestCandidate : NULL;
}

// ==============================================
// Utility: Replace First Occurrence of Token
// ==============================================
void replace_token(const char *line, const char *token, const char *suggestion, char *buffer, size_t size) {
    const char *pos = strstr(line, token);
    if (!pos) {
        strncpy(buffer, line, size);
        buffer[size - 1] = '\0';
        return;
    }
    size_t prefixLen = pos - line;
    if (prefixLen >= size) prefixLen = size - 1;
    strncpy(buffer, line, prefixLen);
    buffer[prefixLen] = '\0';
    strncat(buffer, suggestion, size - strlen(buffer) - 1);
    const char *after_token = pos + strlen(token);
    strncat(buffer, after_token, size - strlen(buffer) - 1);
}

// ==============================================
// Enhanced Error Handling (Displays colored output)
// ==============================================
void maple_error_at_line(int line_number, const char *line, const char *token) {
    const char *suggestion = suggest_command(token);
    char corrected_line[256] = {0};

    printf("Error:\n");
    // Print the erroneous line in red.
    printf("\033[31mLine %d: `%s`\033[0m\n", line_number, line);
    if (suggestion) {
        replace_token(line, token, suggestion, corrected_line, sizeof(corrected_line));
        // Print suggestion in blue.
        printf("\033[34mDid you mean, `%s`?\033[0m\n", corrected_line);
    } else {
        // Generic unknown command message.
        printf("\033[34mUnknown command `%s`.\033[0m\n", token);
    }
}

// ==============================================
// Built-in: Handle "os" Commands
// ==============================================
void handle_os_command(const char *line) {
    // Expect: os.<cmd>(<params>)
    const char *prefix = "os.";
    if (strncmp(line, prefix, strlen(prefix)) != 0) {
        printf("Error: Invalid os command syntax.\n");
        return;
    }
    // Find the opening and closing parentheses.
    const char *openParen = strchr(line, '(');
    const char *closeParen = strchr(line, ')');
    if (!openParen || !closeParen || openParen > closeParen) {
        printf("Error: Invalid syntax in os command.\n");
        return;
    }

    // Extract the sub-command (e.g., pwd, listdir, system)
    int cmdLen = openParen - (line + strlen(prefix));
    char cmd[64] = {0};
    if (cmdLen >= sizeof(cmd))
        cmdLen = sizeof(cmd) - 1;
    strncpy(cmd, line + strlen(prefix), cmdLen);
    cmd[cmdLen] = '\0';

    // Extract parameter, if any.
    int paramLen = closeParen - openParen - 1;
    char param[256] = {0};
    if (paramLen > 0) {
        strncpy(param, openParen + 1, paramLen);
        param[paramLen] = '\0';
    }

    // Handle specific os sub-commands.
    if (strcmp(cmd, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("os.pwd error");
    } else if (strcmp(cmd, "listdir") == 0) {
        const char *dirname = (strlen(param) > 0) ? param : ".";
        DIR *dir = opendir(dirname);
        if (!dir) {
            perror("os.listdir error");
            return;
        }
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    } else if (strcmp(cmd, "system") == 0) {
        if (strlen(param) > 0)
            system(param);
        else
            printf("os.system requires a command parameter.\n");
    } else {
        printf("os: Unknown sub-command `%s`.\n", cmd);
    }
}

// ==============================================
// Built-in: Handle "import" Command
// ==============================================
void handle_import_command(const char *line) {
    // Expected syntax: import <module_name>
    char module[64] = {0};
    if (sscanf(line, "import %63s", module) == 1) {
        printf("Module '%s' imported.\n", module);
    } else {
        printf("Error: Invalid syntax for import command.\n");
    }
}

// ==============================================
// Main Interpreter Loop (Runtime Engine)
// ==============================================
void maple_run(MapleEngine *engine, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        // Remove trailing newline.
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        // Skip empty lines.
        if (strlen(line) == 0) continue;

        char token[64] = {0};
        // Extract the first token.
        if (sscanf(line, "%63s", token) != 1) continue;

        if (strcmp(token, "print") == 0) {
            // Print the remainder of the line.
            char *rest = strchr(line, ' ');
            if (rest)
                printf("%s\n", rest + 1);
            else
                printf("\n");
        } else if (strncmp(line, "os.", 3) == 0) {
            handle_os_command(line);
        } else if (strncmp(line, "import", 6) == 0) {
            handle_import_command(line);
        } else if (strcmp(token, "exit") == 0) {
            printf("Exiting Maple...\n");
            fclose(file);
            exit(0);
        } else {
            // Unknown command – report error with suggestions.
            maple_error_at_line(line_number, line, token);
        }
    }
    fclose(file);
}
