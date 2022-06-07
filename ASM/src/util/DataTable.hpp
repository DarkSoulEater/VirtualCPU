#ifndef ASM_SRC_UTIL_DATATABLE_HPP_
#define ASM_SRC_UTIL_DATATABLE_HPP_

#include "HashTable.hpp"
#include "ByteArray.hpp"

extern HashTable *CmdTable;
extern HashTable *RegTable;
extern HashTable *LabelTable;
extern HashTable *DirTable;

void CreateCommandTable();
void CreateRegisterTable();
void CreateLabelTable();
void CreateDirectiveTable();

struct PendingLabel {
    const char* name;
    long long label;
};

enum DirectiveType {
    db = 10,
    PI = 314
};

#endif // ASM_SRC_UTIL_DATATABLE_HPP_