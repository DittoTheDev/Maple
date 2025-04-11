#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>    // For _getcwd
#include <io.h>        // For _findfirst/_findnext (if needed)
#include "engine/engine.h"

// Define opcodes (must match those in the compiler).
#define OP_PRINT       0x01
#define OP_OS_PWD      0x02
#define OP_OS_LISTDIR  0x03
#define OP_OS_SYSTEM   0x04
#define OP_IMPORT      0x05
#define OP_EXIT        0x06

// ========================
// MapleEngine Structure
// ========================
struct MapleEngine {
    // State, context, etc.
};

// ================================
// Engine Creation and Destruction
// ================================
MapleEngine* maple_create() {
    MapleEngine *engine = (MapleEngine*) malloc(sizeof(MapleEngine));
    return engine;
}

void maple_destroy(MapleEngine *engine) {
    if (engine)
        free(engine);
}

// ====================================
// Interpreter: Run Bytecode File
// ====================================
void maple_run(MapleEngine *engine, const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open bytecode file %s\n", filename);
        return;
    }
    
    int running = 1;
    while (running) {
        unsigned char opcode;
        if (fread(&opcode, sizeof(opcode), 1, file) != 1)
            break;  // End of file.
        
        switch(opcode) {
            case OP_PRINT: {
                // Operand is a null-terminated string.
                char buffer[1024];
                if (fgets(buffer, sizeof(buffer), file)) {
                    // This won't work properly because fgets stops at newline.
                    // Instead, read byte-by-byte until '\0' is encountered.
                    // Here, we do:
                    int pos = 0;
                    char ch;
                    while (fread(&ch, sizeof(ch), 1, file) == 1 && ch != '\0' && pos < sizeof(buffer)-1) {
                        buffer[pos++] = ch;
                    }
                    buffer[pos] = '\0';
                    printf("%s\n", buffer);
                }
                break;
            }
            case OP_OS_PWD: {
                char cwd[1024];
                if (_getcwd(cwd, sizeof(cwd)) != NULL)
                    printf("%s\n", cwd);
                else
                    perror("os.pwd error");
                break;
            }
            case OP_OS_LISTDIR: {
                // Operand: directory (null-terminated string)
                char dir[256];
                int pos = 0;
                char ch;
                while (fread(&ch, sizeof(ch), 1, file) == 1 && ch != '\0' && pos < sizeof(dir)-1) {
                    dir[pos++] = ch;
                }
                dir[pos] = '\0';
                // For simplicity, we call system("dir <dir>") on Windows.
                char command[512];
                if (strlen(dir) == 0)
                    strcpy(dir, ".");
                snprintf(command, sizeof(command), "dir \"%s\"", dir);
                system(command);
                break;
            }
            case OP_OS_SYSTEM: {
                // Operand: command string.
                char cmd[256];
                int pos = 0;
                char ch;
                while (fread(&ch, sizeof(ch), 1, file) == 1 && ch != '\0' && pos < sizeof(cmd)-1) {
                    cmd[pos++] = ch;
                }
                cmd[pos] = '\0';
                system(cmd);
                break;
            }
            case OP_IMPORT: {
                // Operand: module name.
                char module[256];
                int pos = 0;
                char ch;
                while (fread(&ch, sizeof(ch), 1, file) == 1 && ch != '\0' && pos < sizeof(module)-1) {
                    module[pos++] = ch;
                }
                module[pos] = '\0';
                printf("Imported module: %s\n", module);
                break;
            }
            case OP_EXIT: {
                printf("Exiting Maple...\n");
                running = 0;
                break;
            }
            default: {
                fprintf(stderr, "Error: Unknown opcode 0x%02X.\n", opcode);
                running = 0;
                break;
            }
        }
    }
    fclose(file);
}
