#ifndef ASM_SRC_BACKEND_ARGUMENT_HPP_
#define ASM_SRC_BACKEND_ARGUMENT_HPP_

#include "cpu_config.hpp"

struct Argument { 
    ArgumentFlags flags = 0;

    IMMConst    VIConst   = 0;
    Register    VRegister = 0;
    Memory      VMemory   = 0;
    double      VDConst   = 0;
    Label       VLabel    = 0;
    const char* NameLabel = nullptr;
};

#endif // ASM_SRC_BACKEND_ARGUMENT_HPP_