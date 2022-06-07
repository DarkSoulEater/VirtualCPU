#ifndef ASM_SRC_FRONTEND_TOKEN_HPP_
#define ASM_SRC_FRONTEND_TOKEN_HPP_

#include <ctype.h>

enum TokenType {
    Undef,
    Command,
    Iconst,
    Dconst,
    Reg,
    ID,
    Plus,
    Colon,
    LeftBracket,
    RightBracket,
    EndLine,
    EndFile,
    Str,
    Directive,
    Error
};

struct Token {
    TokenType type = TokenType::Undef;
    void *data     = nullptr;

    size_t column = 1;
    size_t line   = 0;
};

const char* GetStrTokenType(TokenType token_type);

#endif // ASM_SRC_FRONTEND_TOKEN_HPP_
