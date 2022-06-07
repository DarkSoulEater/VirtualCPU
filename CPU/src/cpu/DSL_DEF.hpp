#ifndef CPU_SCR_CPU_DSLDEF_HPP_
#define CPU_SCR_CPU_DSLDEF_HPP_

#include "../config.hpp"

#define GLV  (IMMConst*)GetArgument(cpu) 
#define GRV *(IMMConst*)GetArgument(cpu) 
#define GFLV (double*)GLV
#define GFRV *(double*)&GRV

#define VAR(NAME) IMMConst NAME
#define FVAR(NAME) double NAME
                                                
#define POP() Pop(cpu)  
#define PUSH(VAL) Push(cpu, VAL)
#define JMP(LABEL) Jump(cpu, (Label)LABEL)
#define HLT SleepCPU(cpu)
#define CMP(A, B) Cmp(cpu, A, B)
#define CMPF(A, B) Cmpf(cpu, A, B)
#define LOOP(LABEL) Loop(cpu, (Label)LABEL)
#define DRAW() Draw()
#define READ(FILE_NAME, DST) Read((const char*)FILE_NAME, DST)
#define READ_BMP(FILE_NAME, DST) ReadBMP((const char*)FILE_NAME, DST)
#define READ_OBJ(FILE_NAME, V, VS, P, PS) ReadOBJ((const char*)FILE_NAME, V, VS, P, PS)
#define CREATE_WINDOW_SURFACE(W, H) CreateWindowSurface(cpu, W, H)

#define IF(COND) if (COND)
#define RE cpu->EF
#define RNE !cpu->EF
#define RG cpu->GF
#define RGE cpu->GF || cpu->EF
#define RL cpu->LF
#define RLE cpu->LF || cpu->EF

#define OUTS(PTR) Outs((char*)PTR)

#endif // CPU_SCR_CPU_DSLDEF_HPP_