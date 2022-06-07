#ifndef CPU_SRC_CPU_CPU_HPP_
#define CPU_SRC_CPU_CPU_HPP_

#include "cpu_config.hpp"
#include "Program.hpp"

#define DEF_REG(Name, Num, Size, Access) Register Name : Size;
struct CPU {
    bool is_sleep_ = true;
    Register REG[0];
    #include "reg_def.hpp"
};

void ExecuteProgram(CPU *cpu, Program *program);

#endif // CPU_SRC_CPU_CPU_HPP_