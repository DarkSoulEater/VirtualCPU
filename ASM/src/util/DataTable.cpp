#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Lexer.hpp"

#include "DataTable.hpp"


#define DEF_CMD(Name, Num, Carg, Code) {#Name, Num, Carg}, 
const InstructionInfo InstructionsInfo[] {
    #include "cmd_def.hpp"
};

#define DEF_REG(Name, Num, Size, Access) {#Name, Num, Access},
const RegisterInfo RegistersInfo[] {
    #include "reg_def.hpp"
};

HashTable *CmdTable = nullptr;
HashTable *RegTable = nullptr;
HashTable *DirTable = nullptr;

static char* Strdup(const char *src_str) {
    int i = 0;
    for (; isalpha(src_str[i]) || isdigit(src_str[i]) || src_str[i] == '_'; ++i)
        ;

    char *dst_str = (char*) calloc(i + 1, sizeof(char));
    strncpy(dst_str, src_str, i);
    dst_str[i] = '\0';

    return dst_str;
}

void CreateCommandTable() {
    CmdTable = TableAllocate(Strdup);
    for (size_t i = 0; i < sizeof(InstructionsInfo) / sizeof(InstructionInfo); ++i) {
        TableInsert(CmdTable, InstructionsInfo[i].name, (void*)(InstructionsInfo + i));
    }
}

void CreateRegisterTable() {
    RegTable = TableAllocate(Strdup);
    for (size_t i = 0; i < sizeof(RegistersInfo) / sizeof(RegisterInfo); ++i) {
        if (RegistersInfo[i].access) {
            TableInsert(RegTable, RegistersInfo[i].name, (void*)(RegistersInfo +i));
        }
    }
}

void CreateDirectiveTable() {
    DirTable = TableAllocate(Strdup);
    TableInsert(DirTable, "db", (void*)DirectiveType::db);
    TableInsert(DirTable, "PI", (void*)DirectiveType::PI);
}

HashTable *LabelTable = nullptr;

void CreateLabelTable() {
    LabelTable = TableAllocate(Strdup);
}
