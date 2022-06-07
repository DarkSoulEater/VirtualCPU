#include "Token.hpp"

const char* GetStrTokenType(TokenType token_type) {
    switch (token_type) {
    case TokenType::Command:
        return "\"Command Token\"";
        break;
    
    case TokenType::Dconst:
        return "\"Double const Token\"";
        break;

    case TokenType::EndFile:
        return "\"End of file Token\"";
        break;

    case TokenType::EndLine:
        return "\"End of file Token\"";
        break;

    case TokenType::Error:
        return "\"Error Token\"";
        break;

    case TokenType::Iconst:
        return "\"Int const Token\"";
        break;

    case TokenType::ID:
        return "\"Indeficator Token\"";
        break;

    case TokenType::LeftBracket:
        return "\"'[' Token\"";
        break;

    case TokenType::RightBracket:
        return "\"']' Token\"";
        break;

    case TokenType::Reg:
        return "\"Register Token\"";
        break;

    case TokenType::Plus:
        return "\"'+' Token\"";
        break;

    case TokenType::Colon:
        return "\"':' Token\"";
        break;

    case TokenType::Str:
        return "\"':' String\"";
        break;

    case TokenType::Directive:
        return "\"':' Directive\"";
        break;

    case TokenType::Undef:
        return "\"Undefined Token\"";
        break;

    default:
        return "I not know this token";
        break;
    }
}