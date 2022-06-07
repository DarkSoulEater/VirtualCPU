#ifndef CPU_SRC_CPU_PROGRAM_HPP_
#define CPU_SRC_CPU_PROGRAM_HPP_

const size_t kProgramSize = 33554432;
const size_t kVideoMemoryAdress = 16777216;
const size_t kVideoMemorySize = 1920LL * 1080LL * 4LL;

struct Program {
    void *data_;

    void *CS_;
    void *DS_;
};

Program* CreateProgram(const char *fname);

#endif // CPU_SRC_CPU_PROGRAM_HPP_