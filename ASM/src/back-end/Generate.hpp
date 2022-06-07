#ifndef ASM_SRC_BACKEND_GENERATE_HPP_
#define ASM_SRC_BACKEND_GENERATE_HPP_

#include "Argument.hpp"
#include "Lexer.hpp"

void GenerateSignature();

void GenerateArg(Argument *arg);

void GenerateCommand(InstructionInfo *command);

void GenerateNewLablel(const char *label_name);

void GenerateLabel(const char *label_name);

void GenerateStr(const char *str);

void GenerateVar(void* var, size_t size);

void SaveProgramInFile(const char *fname);

#endif // ASM_SRC_BACKEND_GENERATE_HPP_