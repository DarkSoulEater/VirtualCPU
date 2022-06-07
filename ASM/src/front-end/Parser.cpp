#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#include "Buffer.hpp"
#include "Lexer.hpp"
#include "DataTable.hpp"
#include "Argument.hpp"
#include "Generate.hpp"
#include "Parser.hpp"

static const char *FileNames = nullptr;
static bool FileIsCorrect = false;

static Token CurrentToken = {};

static void PrintError(const char *format, ...) {
    va_list message;
	va_start(message, format);
	fprintf(stderr, "ASM: %s:%llu:%llu: lexer error: ", FileNames, CurrentToken.line, CurrentToken.column);
	vfprintf(stderr, format, message);
	putc('\n', stderr);
	va_end(message);
}

static void ParseDirective(Argument *arg = nullptr) {
    DirectiveType dir_type = DirectiveType((long long)CurrentToken.data);

    switch (dir_type) {
    case DirectiveType::db:
        if (arg != nullptr) {
            PrintError("The directive 'db' can't be argument");
            FileIsCorrect = false;
        }
        
        CurrentToken = GetToken();

        if (CurrentToken.type != TokenType::Str) {
            PrintError("After the directive 'db', the line is required");
            FileIsCorrect = false;
        }

        GenerateStr((const char*)CurrentToken.data);

        CurrentToken = GetToken();
        break;

    case DirectiveType::PI:
        arg->flags |= ArgFlags::kDoubleConst;
        arg->VDConst = 3.1415926535;
        CurrentToken = GetToken();
        break;
    
    default:
        PrintError("Use undefined directive\n");
        FileIsCorrect = false;
        break;
    }
}

static void ParseRegisterArg(Argument *arg);

static void ParseIconstArg(Argument *arg) {
    assert(arg);

    if (arg->flags & ArgFlags::kIntConst) {
        PrintError("redefinition of int const flags");
        FileIsCorrect = false;
        return;
    }

    arg->flags |= ArgFlags::kIntConst;

    if (sscanf((char*)CurrentToken.data, "%lld", &arg->VIConst) == 0) {
        PrintError("cloud not read the int argument");
        FileIsCorrect = false;
        return;
    }

    CurrentToken = GetToken();
    if (CurrentToken.type == TokenType::Plus) {
        CurrentToken = GetToken();
        if (CurrentToken.type == TokenType::Reg) {
            ParseRegisterArg(arg);
        } else {
            PrintError("expected %s, but found %s", GetStrTokenType(TokenType::Reg), GetStrTokenType(CurrentToken.type));
            FileIsCorrect = false;
        return;
        }
    }
}

static void ParseDconstArg(Argument *arg) {
    if (arg->flags & ArgFlags::kDoubleConst) {
        PrintError("redefinition of double const flags");
        FileIsCorrect = false;
        return;
    }

    arg->flags |= ArgFlags::kDoubleConst;

    if (sscanf((char*)CurrentToken.data, "%lf", &arg->VDConst) == 0) {
        PrintError("cloud not read the double argument");
        FileIsCorrect = false;
        return;
    }

    CurrentToken = GetToken();
}

static void ParseRegisterArg(Argument *arg) {
    assert(arg);

    if (arg->flags & ArgFlags::kRegister) {
        PrintError("redefinition of register const flags");
        FileIsCorrect = false;
        return;
    }

    arg->flags |= ArgFlags::kRegister;

    arg->VRegister = ((RegisterInfo*)(CurrentToken.data))->ID;

    CurrentToken = GetToken();
    if (CurrentToken.type == TokenType::Plus) {
        CurrentToken = GetToken();
        if (CurrentToken.type == TokenType::Iconst) {
            ParseIconstArg(arg);
        } else {
            PrintError("expected %s, but found %s", GetStrTokenType(TokenType::Iconst), GetStrTokenType(CurrentToken.type));
            FileIsCorrect = false;
            return;
        }
    }
}

static void ParseLabelArg(Argument *arg) {
    assert(arg);

    if (arg->flags & ArgFlags::kLabel) {
        PrintError("redefinition of label flags");
        FileIsCorrect = false;
        return;
    }

    arg->flags |= ArgFlags::kLabel;
    arg->VLabel = 0;
    arg->NameLabel = (const char*)CurrentToken.data; 

    CurrentToken = GetToken();
}

static void ParseMemoryArg(Argument *arg) {
    assert(arg);

    if (arg->flags & ArgFlags::kMemory) {
        PrintError("redefinition of memory const flags");
        FileIsCorrect = false;
        return;
    }

    arg->flags |= ArgFlags::kMemory;

    CurrentToken = GetToken();
    switch (CurrentToken.type) {
    case TokenType::Iconst:
        ParseIconstArg(arg);
        break;
    
    case TokenType::Reg:
        ParseRegisterArg(arg);
        break;

    case TokenType::ID:
        ParseLabelArg(arg);
        break;

    case TokenType::Command: 
    case TokenType::Dconst: 
    case TokenType::Plus: 
    case TokenType::Colon:
    case TokenType::LeftBracket: 
    case TokenType::RightBracket:
    case TokenType::EndFile:
    case TokenType::EndLine:
    case TokenType::Str:
    case TokenType::Directive:
    case TokenType::Error:
    case TokenType::Undef:
    default:
        PrintError("expected %s OR %s, but found %s", GetStrTokenType(TokenType::Iconst), 
                                                      GetStrTokenType(TokenType::Reg),
                                                      GetStrTokenType(CurrentToken.type));
        FileIsCorrect = false;
        return;
        break;
    }

    if (CurrentToken.type == TokenType::RightBracket) {
        CurrentToken = GetToken();
    } else {
        PrintError("expected %s, but found %s", GetStrTokenType(TokenType::RightBracket), GetStrTokenType(CurrentToken.type));
        FileIsCorrect = false;
        return;
    }
}

static void ParseArgument(Argument *arg) { 
    assert(arg);

    switch (CurrentToken.type) {
    case TokenType::LeftBracket:
        ParseMemoryArg(arg);
        break;
    
    case TokenType::Reg:
        ParseRegisterArg(arg);
        break;

    case TokenType::Iconst:
        ParseIconstArg(arg);
        break;

    case TokenType::Dconst:
        ParseDconstArg(arg);
        break;

    case TokenType::ID:
        ParseLabelArg(arg);
        break;

    case TokenType::Directive:
        ParseDirective(arg);
        break;

    case TokenType::Plus:
    case TokenType::EndLine:
    case TokenType::EndFile:
    case TokenType::Command:
    case TokenType::Colon:
    case TokenType::RightBracket:
    case TokenType::Str:
    case TokenType::Error:
    case TokenType::Undef:

    default:
        PrintError("Expected argument, but found %s", GetStrTokenType(CurrentToken.type));
        FileIsCorrect = false;
        return;
        break;
    }
}

static void ParseArguments(int argc) {
    assert(argc >= 0);

    for (int i = 0; i < argc; ++i) {
        Argument arg = {};
        ParseArgument(&arg);
        GenerateArg(&arg);
    }

    if (CurrentToken.type == TokenType::EndLine || CurrentToken.type == TokenType::EndFile) {
        CurrentToken = GetToken();
        return;
    }
    
    PrintError("Expected end of line, but found %s", GetStrTokenType(CurrentToken.type));
    FileIsCorrect = false;
    return;
}

static void ParseLabel() {
    Token prev_tk = CurrentToken;
    CurrentToken = GetToken();
    if (CurrentToken.type != TokenType::Colon) {
        PrintError("Expected %s, but found %s", GetStrTokenType(TokenType::Colon), GetStrTokenType(CurrentToken.type));
        FileIsCorrect = false;
        return;
    }

    CurrentToken = GetToken();
    if (CurrentToken.type == TokenType::EndLine || CurrentToken.type == TokenType::EndFile) {
        CurrentToken = GetToken();
        GenerateNewLablel((const char*)prev_tk.data);
        return;
    }

    PrintError("Expected end of line, but found %s", GetStrTokenType(CurrentToken.type));
    FileIsCorrect = false;
    return;
}

static void ParseCommand() {
    InstructionInfo instruction_info = *(InstructionInfo*)CurrentToken.data;
    GenerateCommand(&instruction_info);
    CurrentToken = GetToken();
    ParseArguments(instruction_info.argc);
}

static void CreateVariable() {
    if (CurrentToken.type == TokenType::Iconst) {
        IMMConst var = 0;
        if (sscanf((char*)CurrentToken.data, "%lld", &var) == 0) {
            PrintError("cloud not read the int argument");
            FileIsCorrect = false;
            return;
        }
        GenerateVar(&var,  sizeof(IMMConst));
    } else if (CurrentToken.type == TokenType::Dconst) {
        double var = 0;
        if (sscanf((char*)CurrentToken.data, "%lf", &var) == 0) {
            PrintError("cloud not read the double argument");
            FileIsCorrect = false;
            return;
        }
        GenerateVar(&var, sizeof(double));
    }

    CurrentToken = GetToken();

    if (CurrentToken.type == TokenType::EndLine || CurrentToken.type == TokenType::EndFile) {
        CurrentToken = GetToken();
        return;
    }

    PrintError("Expected end of line, but found %s", GetStrTokenType(CurrentToken.type));
    FileIsCorrect = false;
    return;
}

static void Parse() {
    if (!FileIsCorrect) {
        return;
    }

    switch (CurrentToken.type) {
    case TokenType::Command:
        ParseCommand();
        break;
    
    case TokenType::ID:
        ParseLabel();
        break;

    case TokenType::Directive:
        ParseDirective();
        break;

    case TokenType::Iconst:
    case TokenType::Dconst:
        CreateVariable();
        break;

    case TokenType::EndFile: {
        char* out_file = (char*) FileNames;
        for (int i = 0;;++i) {
            if (out_file[i] == '\0') {
                out_file[i - 2] = 'b';
                out_file[i - 1] = 't';
                break;
            }
        }
        SaveProgramInFile(out_file);
        return;
        break;
    }

    case TokenType::EndLine:
        CurrentToken = GetToken();
        break;

    case TokenType::Reg:
    case TokenType::Plus:
    case TokenType::Colon:
    case TokenType::LeftBracket:
    case TokenType::RightBracket:
    case TokenType::Str:
    case TokenType::Error:
    case TokenType::Undef:
    default:
        PrintError("Expected instruction. but found %s", GetStrTokenType(CurrentToken.type));
        FileIsCorrect = false;
        return;
        break;
    }

    Parse();
}

void ParseFile(const char *fname) {
    FileIsCorrect = true;

    Buffer *file_buffer = CreateBufferFromFile(fname);
    InitLexer((const char *)BufferGetData(file_buffer), fname);
    GenerateSignature();
    FileNames = fname;

    CurrentToken = GetToken();
    Parse();
}