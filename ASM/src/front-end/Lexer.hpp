#ifndef ASM_SRC_FRONTEND_LEXER_HPP_
#define ASM_SRC_FRONTEND_LEXER_HPP_

#include "Token.hpp"
#include "cpu_config.hpp"

struct InstructionInfo {
    const char* name;
    Instruction ID;
    int argc;
};

struct RegisterInfo {
    const char* name;
    RegisterID ID;
    bool access;
};

void InitLexer(const char *data, const char *fname);

Token GetToken();

#endif // ASM_SRC_FRONTEND_LEXER_HPP_