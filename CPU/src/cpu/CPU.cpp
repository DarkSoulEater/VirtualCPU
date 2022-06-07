#include <assert.h>
#include <cstdint>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <windows.h>
#include <cmath>

#include "Program.hpp"
#include "config.hpp"
#include "Core.hpp"
#include "cpu_config.hpp"
#include "CPU.hpp"

static Core* GraphicsCore;

#if defined(DEBUD_ACTIVATE_DUMP)
    static Core DumpCore(1920, 1080);
#endif

#define DEF_REG(Name, Num, Size, Access) Name = Num,
enum Reg { 
    #include "reg_def.hpp"
};

#define GETIP(TYPE)              \
    *(TYPE*)(cpu->CS + cpu->IP); \
    cpu->IP += sizeof(TYPE);

IMMConst GetArgument(CPU *cpu) {
    static IMMConst arg;
    arg = 0;

    ArgumentFlags flags = GETIP(ArgumentFlags);

    if (flags == ArgFlags::kRegister) {
        RegisterID reg = GETIP(RegisterID);
        return (IMMConst)&cpu->REG[reg];
    }

    if (flags & ArgFlags::kIntConst) {
        arg += GETIP(IMMConst);
    }

    if (flags & ArgFlags::kDoubleConst) {
        arg += GETIP(IMMConst);
    }

    if (flags & ArgFlags::kRegister) {
        RegisterID reg_id = GETIP(RegisterID);
        arg += cpu->REG[reg_id];
    }

    if (flags == (ArgFlags::kIntConst | ArgFlags::kRegister) || flags == ArgFlags::kIntConst || flags == ArgFlags::kDoubleConst) {
        return (IMMConst)&arg;    
    }

    if (flags == (ArgFlags::kMemory | ArgFlags::kLabel)) {
        Label label = GETIP(Label);
        arg += label;
        return (IMMConst)&arg;  
    }

    if (flags & ArgFlags::kMemory) {
        arg += cpu->MS;
    }

    if (flags & ArgFlags::kLabel) {
        Label label = GETIP(Label);
        arg += label + cpu->MS;
    }


    return arg;
}

static void Jump(CPU *cpu, Label label) {
    cpu->IP = label - cpu->CS;

    DEB(printf("id -> %lld\n", cpu->IP));
}

static void Push(CPU *cpu, IMMConst value) {
    cpu->SP += sizeof(IMMConst);
    *(IMMConst*)(cpu->SS - cpu->SP) = value;

    DEB(printf("--push: %lld\n", value));
}

static IMMConst Pop(CPU *cpu) {
    DEB(printf("--pop: %lld\n", *(IMMConst*)(cpu->CS + cpu->SP)))

    cpu->SP -= sizeof(IMMConst);
    return *(IMMConst*)(cpu->SS - cpu->SP - sizeof(IMMConst));
}

Instruction GetCommand(CPU *cpu) {
    cpu->IP += sizeof(Instruction);
    return *((Instruction*)(cpu->CS + cpu->IP - sizeof(Instruction)));
}

static void Outs(char *str) {
    printf("%s\n", str);
}

static void Cmp(CPU *cpu, IMMConst L, IMMConst R) {
    if (L == R) cpu->EF = 1;
    else cpu->EF = 0;
    if (L < R)  cpu->LF = 1;
    else cpu->LF = 0;
    if (L > R)  cpu->GF = 1;
    else cpu->GF = 0;
}

static void Cmpf(CPU *cpu, double L, double R) {
    const double kEps = 1e-10;
    if (fabs(R - L) < kEps) cpu->EF = 1;
    else cpu->EF = 0;
    if (L < R - kEps) cpu->LF = 1;
    else cpu->LF = 0;
    if (L - kEps > R) cpu->GF = 1;
    else cpu->GF = 0;
}

static void Loop(CPU *cpu, Label label) {
    cpu->CX -= 1;
    if (cpu->CX != 0LL) {
        Jump(cpu, label);
    }
}

static void Draw() {
    GraphicsCore->Update();
}

static void CreateWindowSurface(CPU *cpu, IMMConst width, IMMConst height) {
    GraphicsCore = new Core(width, height);
    GraphicsCore->SetCPU(cpu, VideoMode::VideoMemmory);
}

static FILE* OpenFile(const char* fname, int* file_size) {
    FILE *src = nullptr;
    if (fopen_s(&src, fname, "rb")) {
        perror("Could not read the source code of the program");
        return nullptr;
    }

    if (file_size != nullptr) {
    struct stat file_stat = {};
        if (fstat(fileno(src), &file_stat)) {
            perror("Error: failed to read file parameters");
            fclose(src);
            return nullptr;
        }
        *file_size = file_stat.st_size;
    }

    return src;
}

static void Read(const char* fname, void* dst) {
    int file_size = 0;
    FILE* src = OpenFile(fname, &file_size);

    size_t result = fread(dst, sizeof(char), file_size, src);
    
    if (result != (size_t)file_size) {
        perror("Could not read the whole file");
    }
    fclose(src);
}

static void ReadBMP(const char* fname, void* dst) {
    int file_size = 0;
    FILE* src = OpenFile(fname, &file_size);

    char* pixels = (char*)dst;
    for (int i = 0; i < 10; ++i) {
        getc(src);
    }

    int target = getc(src);
    for (int i = 10; i < target - 1; ++i) {
        getc(src);
    }

    file_size -= target;

    int buf_size = file_size / 3 * 4;
    int i = 0, j = file_size;
    while (j > 0) {
            pixels[buf_size - 1 - i - 1] = getc(src);
            pixels[buf_size - 1 - i - 2] = getc(src);
            pixels[buf_size - 1 - i - 3] = getc(src);
            pixels[buf_size - 1 - i] = 255;
            j -= 3;
            i += 4;
    }

    fclose(src);
}

static void ReadOBJ(const char* fname, void *vert, void *vert_size, void *polig, void *polyg_size) {
    int file_size = 0;
    FILE* src = OpenFile(fname, &file_size);

    double* vertices_buffer = (double*)vert + 3;
    IMMConst* index_buffer = (IMMConst*)polig;
    IMMConst* vertices_size = (IMMConst*)vert_size;
    IMMConst* polygon_szie = (IMMConst*)polyg_size;

    ++*vertices_size;

    char c = ' ';
    while (fscanf(src, "%c", &c) != EOF) {
        if (c == 'v') {
            ++*vertices_size;
            double X = 0, Y = 0, Z = 0;
            fscanf(src, "%lf %lf %lf", &X, &Y, &Z);
            *vertices_buffer++ = X;
            *vertices_buffer++ = Y;
            *vertices_buffer++ = Z;
        } else if (c == 'f') {
            ++*polygon_szie;
            IMMConst I1 = 0, I2 = 0, I3 = 0;
            fscanf(src, "%lld / %*d %lld / %*d %lld / %*d", &I1, &I2, &I3);
            *index_buffer++ = I1;
            *index_buffer++ = I2;
            *index_buffer++ = I3;
        }
    }
    

    fclose(src);
}

void SleepCPU(CPU *cpu) {
    cpu->is_sleep_ = true;
    while (GraphicsCore && GraphicsCore->Update())
        ;
}

#include "DSL_DEF.hpp"
void ExecuteInstruction(CPU *cpu, Instruction command) {
    switch (command) {
    #define DEF_CMD(Name, Num, Carg, Code)  \
    case Num:                               \
        Code                                \
        break;                              \
    
    #include "cmd_def.hpp"

    default:
        printf("Error: Unknow command"); // Debug
        SleepCPU(cpu);
        break;
    }

    // For smooth rendering (Not CPU)
    static int cnt = 0;
    if (cnt++ / 10000) {
        cnt = 0;

        if (GraphicsCore) {
            GraphicsCore->Update();
        }
    }
    // End of smooth rendering
}
#include "DSL_UNDEF.hpp"

void WakeUpCPU(CPU *cpu) {
    cpu->is_sleep_ = false;

    #if defined(DEBUD_ACTIVATE_DUMP)
        DumpCore.SetCPU(cpu, VideoMode::Dump);
    #endif


    while(!cpu->is_sleep_) {
        Instruction command = GetCommand(cpu);
        ExecuteInstruction(cpu, command);


#if defined(DEBUD_ACTIVATE_DUMP)
    static int cmd_cnt = 0;
    
    if (++cmd_cnt >= kLenBreakPoint) {
        DumpCore.Update();
        printf("Enter...\n");
        getc(stdin);
        cmd_cnt = 0;
    }
#endif

    }
}

void ExecuteProgram(CPU *cpu, Program *program) {
    cpu->CS = (Register)program->CS_;         // Code  Segment
    cpu->DS = (Register)program->DS_;         // Data  Segment
    cpu->SS = (Register)program->CS_ + 10000; // Stack Segment // TODO:
    cpu->MS = (Register)program->data_;       // Memory Segment
    cpu->VS = (Register)kVideoMemoryAdress;   // Video memory Segment

    cpu->IP = 0; // Instruction Pointer
    cpu->SP = 0; // Base Pointer

    WakeUpCPU(cpu);
}

