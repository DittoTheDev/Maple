#ifndef COMPILER_H
#define COMPILER_H

// Compile .maple source code into .mbc bytecode.
int maple_compile(const char* source_filename, const char* output_filename);

#endif // COMPILER_H
