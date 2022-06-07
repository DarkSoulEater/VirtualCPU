#ifndef CPU_CONFIG_CPUCONFIG_HPP_
#define CPU_CONFIG_CPUCONFIG_HPP_
// Сигнатура

const int kFileFormat = 1145390145; // 'DEDA'
const int kLVersion   = (10 << 16) + 1;

// Define GetLVer GerRVer

typedef unsigned char Instruction;
typedef unsigned char RegisterID;
typedef unsigned long long Register;
typedef unsigned long long Memory;
typedef unsigned long long Label;
typedef long long IMMConst;
typedef unsigned char ArgumentFlags;

// Argument flags:
enum ArgFlags {
    kIntConst    = (1 << 0),
    kRegister    = (1 << 1),
    kMemory      = (1 << 2),
    kLabel       = (1 << 3),
    kDoubleConst = (1 << 4)
};

#endif // CPU_CONFIG_CPUCONFIG_HPP_
