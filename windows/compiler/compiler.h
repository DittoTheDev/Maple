#ifndef COMPILER_H
#define COMPILER_H

// Compile .maple source code into .mbc bytecode.
// Returns 0 on success and nonzero on failure.
int maple_compile(const char* source_filename, const char* output_filename);

#endif // COMPILER_H
