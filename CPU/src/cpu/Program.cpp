#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cpu_config.hpp"

#include "Program.hpp"

Program* CreateProgram(const char *fname) {
    assert(fname);

    FILE *src = nullptr;
    if (fopen_s(&src, fname, "rb")) {
        perror("could not read the source code of the program");
        return nullptr;
    }

    struct stat file_stat = {};
    if (fstat(fileno(src), &file_stat)) {
        perror("Error: failed to read file parameters");
        fclose(src);
        return nullptr;
    }

    Program *prog = (Program*) calloc(1, sizeof(Program));
    if (!prog) {
        perror("Failed to allocate Program");
        fclose(src);
        return nullptr;
    }

    prog->CS_ = prog->data_ = calloc(1, kProgramSize);
    if (!prog->data_) {
        perror("Failed to allocate Program data");
        free(prog);
        fclose(src);
        return nullptr;
    }


    size_t result = fread(prog->data_, sizeof(char), file_stat.st_size, src);
    if (result != (size_t)file_stat.st_size) {
        perror("Could not read the whole file");
        free(prog->data_);
        free(prog);
        fclose(src);
        return nullptr;
    }

    fclose(src);

    int file_format = *(int*)prog->data_;
    if (file_format != kFileFormat) {
        fprintf(stderr, "Wrong file format\n");
        abort();
    }

    int file_version = *(int*)((int*)prog->data_ + 1);
    if ((file_version >> 16) != (kLVersion >> 16)) {
        fprintf(stderr, "File version does not match CPU version\n");
        abort();
    }

    prog->DS_ = (void*)0;
    prog->CS_ = (char*)prog->data_ + sizeof(kFileFormat) + sizeof(kLVersion); 
    
    return prog;
}