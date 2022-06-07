#include <stdio.h>
#include <iostream>

#include "CPU.hpp"

void ParseArg(const int argc, const char* argv[]);

int main(const int argc, const char* argv[]) {
    ParseArg(argc, argv);

    CPU cpu = {};

    Program *prog = CreateProgram(argv[1]);
    ExecuteProgram(&cpu, prog);
}


void ParseArg(const int argc, const char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        //printf("%s ", argv[i]);
    }
    //printf("\n\n");
}
