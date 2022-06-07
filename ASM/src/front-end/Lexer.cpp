#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "Lexer.hpp"
#include "DataTable.hpp"

static const char *FileName = nullptr;
static const char *SrcData = nullptr;

static int Column = 1;
static int Line   = 1;

static void PrintError(const char *format, ...) {
    va_list message;
	va_start(message, format);
	fprintf(stderr, "ASM:%s:%d:%d: lexer error: ", FileName, Line, Column);
	vfprintf(stderr, format, message);
	putc('\n', stderr);
	va_end(message);
}

void InitLexer(const char *data, const char *fname) {
    FileName = fname;
    SrcData  = data;
    Column = 1;
    Line   = 1;

    if (CmdTable == nullptr) {
        CreateCommandTable();
    }

    if (RegTable == nullptr) {
        CreateRegisterTable();
    }

    if (LabelTable == nullptr) {
        CreateLabelTable();
    }

    if (DirTable == nullptr) {
        CreateDirectiveTable();
    }
}

static char Getc() {
    if (*SrcData == '\0') return '\0';

    ++SrcData;

    if (*SrcData == '\n') {
        Column = 1, ++Line;
    } else {
        ++Column;
    }

    return *SrcData;
}

static void ConsumeSpace() {
    char character = *SrcData;
    while (isspace(character) && character != '\n' && character != '\0') {
        character = Getc();
    }
}

static void ConsumeComment() {
    char character = *SrcData;
    if (character == ';') {
        while (character != '\n' && character != '\0') {
            character = Getc();
        }
    }
}

static void ConsumeConst(Token *token) {
    char character = *SrcData;
    int point_cnt = 0;

    if (character == '-') character = Getc();
    
    while (isdigit(character) || character == '.') {
        if (character == '.') {
            ++point_cnt;
            token->type = TokenType::Dconst;
        }
        character = Getc();
    }

    if (point_cnt > 1) {
        perror("To much point in double");
        exit(1000); // TODO:
    }
}

static void ConsumeID() {
    char character = *SrcData;
    while (isalpha(character) || isdigit(character) || character == '_') {
        character = Getc();
    }
}

static void ConsumeStr() {
    char character = *SrcData;
    while (character != '"') {
        character = Getc();
    }
    Getc();
}

Token GetToken() {
    assert(SrcData);

    Token token= {};

    ConsumeSpace();
    ConsumeComment();

    token.column = Column;
    token.line   = Line;

    char character = *SrcData;

    if (isalpha(character) || character == '_') {
        if ((token.data = TableLookup(CmdTable, SrcData)) != nullptr) {
            token.type = TokenType::Command;
        } else if ((token.data = TableLookup(RegTable, SrcData)) != nullptr) {
            token.type = TokenType::Reg;
        } else if ((token.data = TableLookup(DirTable, SrcData)) != nullptr) {
            token.type = TokenType::Directive;
        } else {
            token.type = TokenType::ID;
            token.data = (void*) SrcData;
        }

        ConsumeID();

        goto LEXER_SKIP_SWITCH;
    }
    
    if (isdigit(character) || character == '-') {
        token.type = TokenType::Iconst;
        token.data = (void*) SrcData;
        ConsumeConst(&token);

        goto LEXER_SKIP_SWITCH;
    }

    switch (character) {
    case '+':
        Getc();
        token.type = TokenType::Plus;
        break;

    case ':':
        Getc();
        token.type = TokenType::Colon;
        break;

    case '[':
        Getc();
        token.type = TokenType::LeftBracket;
        break;
    
    case ']':
        Getc();
        token.type = TokenType::RightBracket;
        break;

    case '"':
        Getc();
        token.type = TokenType::Str;
        token.data = (void*) SrcData;
        ConsumeStr();
        break;

    case '\n':
        Getc();
        token.type = TokenType::EndLine;
        break;

    case '\0':
        token.type = TokenType::EndFile;
        break;

    default:
        PrintError("Invalid character '%c'", character);
        token.type = TokenType::Error;
        break;
    }


    LEXER_SKIP_SWITCH:
    return token;
}
