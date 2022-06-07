#include <assert.h>

#include "Generate.hpp"
#include "Argument.hpp"
#include "Lexer.hpp"
#include "ByteArray.hpp"
#include "cpu_config.hpp"
#include "DataTable.hpp"

#include <stdio.h>

static ByteArray ProgramArray = {};
static ByteArray LabelArray   = {};

void GenerateSignature() {
    PushBack(&ProgramArray, &kFileFormat, sizeof(kFileFormat));
    PushBack(&ProgramArray, &kLVersion, sizeof(kLVersion));
}

void GenerateLabel(const char *label_name) {
    PendingLabel label = {label_name, (int)ProgramArray.size_};
    PushBack(&LabelArray, &label, sizeof(label));
}

void GenerateArg(Argument *arg) {
    PushBack(&ProgramArray, &arg->flags, sizeof(ArgumentFlags));

    if (arg->flags & ArgFlags::kIntConst) {
        PushBack(&ProgramArray, &arg->VIConst, sizeof(IMMConst));
    }

    if (arg->flags & ArgFlags::kRegister) {
        PushBack(&ProgramArray, &arg->VRegister, sizeof(RegisterID));
    }

    if (arg->flags & ArgFlags::kMemory) {
        // None
    }

    if (arg->flags & ArgFlags::kLabel) {
        GenerateLabel(arg->NameLabel);
        PushBack(&ProgramArray, &arg->VLabel, sizeof(Label));
    }

    if (arg->flags & ArgFlags::kDoubleConst) {
        PushBack(&ProgramArray, &arg->VDConst, sizeof(IMMConst));
    }
}

void GenerateCommand(InstructionInfo *command) {
    PushBack(&ProgramArray, &command->ID, sizeof(command->ID));
}

void GenerateNewLablel(const char *label_name) {
    TableInsert(LabelTable, label_name, (void*)ProgramArray.size_);
    /*if (!TableInsert(LabelTable, label_name, (void*)GetArraySize(&ProgramArray))) { // TODO:
        fprintf("Label with the %s name already exists ", label_name);
        exit(10101); // TODO:
    }*/
}


static void ActivateLabel() {
    if (GetArraySize(&LabelArray) == 0) {
        return;
    }

    PendingLabel *plb = (PendingLabel*) ByteArrayAt(&LabelArray, 0); 

    size_t arr_size = GetArraySize(&LabelArray);
    for (size_t i = 0; i < arr_size / sizeof(PendingLabel); ++i) {
        Label label = (Label)TableLookup(LabelTable, plb->name);

        Label* plb_lable = (Label*)((char*)ProgramArray.data_ + plb->label);
        *plb_lable = label;
        ++plb;
    }

}

void GenerateStr(const char *str) {
    int cnt_byte = 0;
    while (*str != '"') {
        PushBack(&ProgramArray, str++, sizeof(char));
        ++cnt_byte;
    }

    char tmp = '\0';
    PushBack(&ProgramArray, &tmp, sizeof(char));
    ++cnt_byte;
    
    while (cnt_byte % sizeof(IMMConst) != 0) {
        PushBack(&ProgramArray, &tmp, sizeof(char));
        ++cnt_byte;
    }
}

void GenerateVar(void* var, size_t size) {
    PushBack(&ProgramArray, var, size);
}


void SaveProgramInFile(const char *fname) {
    assert(fname);

    ActivateLabel();

    FILE *out_file = nullptr;
    if (fopen_s(&out_file, fname, "wb")) {
        fprintf(stderr, "error: could not open file (%s)", fname);
        perror(" ");
        exit(10); // TODO:
    }

    //ByteArrayClear(&ProgramArray);
    //ByteArrayClear(&LabelArray);
    //TableDeallocate(&LabelTable);
    //TableDeallocate(&CmdTable);
    //TableDeallocate(&RegTable);

    fwrite(ProgramArray.data_, 1, ProgramArray.size_, out_file);
}
