#include <stdio.h>

#include "front-end\Parser.hpp"

void ParseArg(const int argc, const char* argv[]);

int main(const int argc, const char* argv[]) {
    ParseArg(argc, argv);
    
    for (int i = 1; i < argc; ++i) {
        ParseFile(argv[i]);
    }
}


void ParseArg(const int argc, const char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        //printf("%s ", argv[i]);
    }
    //printf("\n\n");
}
