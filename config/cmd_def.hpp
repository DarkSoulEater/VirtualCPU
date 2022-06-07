// #define DEF_CMD(Name, Num, Carg, Code)

#define Operation(OP)   \
    VAR(*L) = GLV;      \
    VAR(R)  = GRV;      \
    *L OP R;

#define OperationF(OP)  \
    FVAR(*L) = GFLV;    \
    FVAR(R)  = GFRV;    \
    *L OP R;   

#define FunctionF(func) \
    FVAR(*L) = GFLV;    \
    FVAR(R)  = GFRV;    \
    *L = func(R);           


DEF_CMD(mov, 0, 2, {
    Operation(=);
    DEB(printf("mov: %lld -> %p\n", R, L));
})           

DEF_CMD(push, 1, 1, {
    VAR(R) = GRV;
    PUSH(R);
    DEB(printf("push: %lld\n", R));
})

DEF_CMD(pop, 2, 1, {
    VAR(*L) = GLV;
    *L = POP();
    DEB(printf("pop: %lld in %p \n", *L, L)); 
})

DEF_CMD(add, 3, 2, {
    Operation(+=);
    DEB(printf("add: %p += %lld. res: %lld\n", L, R, *L));
})

DEF_CMD(sub, 4, 2, {
    Operation(-=);
    DEB(printf("sub: %p -= %lld. res: %lld\n", L, R, *L));
})

DEF_CMD(mul, 5, 2, {
    Operation(*=);
    DEB(printf("mul: %p *= %lld. res: %lld\n", L, R, *L));
})

DEF_CMD(div, 6, 2,{
    Operation(/=);
    DEB(printf("div: %p /= %lld. res: %lld\n", L, R, *L));
})

DEF_CMD(in, 7, 1, {
    DEB(printf("in\n"));
    scanf("%lld", GLV);
})

DEF_CMD(out, 8, 1, {
    DEB(printf("out "));
    printf("%lld\n", GRV);
})

DEF_CMD(cmp, 9, 2, {
    VAR(A) = GRV;
    VAR(B) = GRV;

    CMP(A, B);

    DEB(printf("cmp: %lld  %lld\n", A, B));
})

DEF_CMD(jmp, 10, 1, {
    DEB(printf("jmp: "));
    JMP(GLV);
})

DEF_CMD(je, 11, 1, {
    VAR(*L) = GLV;
    IF(RE) {
        DEB(printf("je: "));
        JMP(L);
    }
})

DEF_CMD(jne, 12, 1, {
    VAR(*L) = GLV;
    IF(RNE) {
        DEB(printf("jne: "));
        JMP(L);
    }
})

DEF_CMD(jg, 13, 1, {
    VAR(*L) = GLV;
    IF(RG) {
        DEB(printf("jg: "));
        JMP(L);
    }
})

DEF_CMD(jge, 14, 1, {
    VAR(*L) = GLV;
    IF(RGE) {
        DEB(printf("jge: "));
        JMP(L);
    }
})

DEF_CMD(jl, 15, 1, {
    VAR(*L) = GLV;
    IF(RL) {
        DEB(printf("jl: "));
        JMP(L);
    }
})

DEF_CMD(jle, 16, 1, {
    VAR(*L) = GLV;
    IF(RLE) {
        DEB(printf("jle: "));
        JMP(L);
    }
})

DEF_CMD(loop, 17, 1, {
    DEB(printf("loop \n"));
    LOOP(GLV);
})

DEF_CMD(call, 18, 1, {
    DEB(printf("call\n"));
    VAR(*L) = GLV;
    PUSH(cpu->IP + cpu->CS);
    JMP(L);
})

DEF_CMD(ret, 19, 0, {
    DEB(printf("ret\n"));
    JMP(POP());
})

DEF_CMD(hlt, 20, 0, {
    DEB(printf("hlt\n"));
    HLT;
})


#define DRW_CMD_BEG 21
DEF_CMD(draw, DRW_CMD_BEG, 0, {
    DRAW();
    DEB(printf("draw\n"));
})

DEF_CMD(cws, DRW_CMD_BEG + 1, 2, {
    VAR(W) = GRV;
    VAR(H) = GRV;
    CREATE_WINDOW_SURFACE(W, H);
    DEB(printf("create window surface: %lld x %lld\n", W, H));
})

DEF_CMD(set, DRW_CMD_BEG + 2, 2, {
    VAR(*L) = GLV;      
    VAR(R)  = GRV;      
    *(int*)L = int(R);
    DEB(printf("set: %d -> %p\n", R, L));
})

#define FLT_CMD_BEG DRW_CMD_BEG + 3

DEF_CMD(outf, FLT_CMD_BEG, 1, {
    DEB(printf("outf "));
    printf("%lf\n", GFRV);
})

DEF_CMD(addf, FLT_CMD_BEG + 1, 2, {
    OperationF(+=);
    DEB(printf("addf: %p += %lf. res: %lf\n", L, R, *L));
})

DEF_CMD(subf, FLT_CMD_BEG + 2, 2, {
    OperationF(-=);
    DEB(printf("subf: %p -= %lf. res: %lf\n", L, R, *L));
})

DEF_CMD(mulf, FLT_CMD_BEG + 3, 2, {
    OperationF(*=);
    DEB(printf("mulf: %p *= %lf. res: %lf\n", L, R, *L));
})

DEF_CMD(divf, FLT_CMD_BEG + 4, 2,{
    OperationF(/=);
    DEB(printf("divf: %p /= %lf. res: %lf\n", L, R, *L));
})

DEF_CMD(sqrtf, FLT_CMD_BEG + 5, 2,{
    FunctionF(sqrt);
    DEB(printf("sqrtf: %p = sqrt(%lf). res: %lf\n", L, R, *L));
})

DEF_CMD(sinf, FLT_CMD_BEG + 6, 2,{
    FunctionF(sin);
    DEB(printf("sinf: %p = sin(%lf). res: %lf\n", L, R, *L));
})

DEF_CMD(cosf, FLT_CMD_BEG + 7, 2,{
    FunctionF(cos);
    DEB(printf("cosf: %p = cos(%lf). res: %lf\n", L, R, *L));
})

DEF_CMD(toi, FLT_CMD_BEG + 8, 2,{
    VAR(*L) = GLV;
    FVAR(R)  = GFRV;
    *L = IMMConst(R);
    DEB(printf("toi: %lf to %d in: %p\n", R, *L, L));
})

DEF_CMD(tof, FLT_CMD_BEG + 9, 2,{
    FVAR(*L) = GFLV;
    VAR(R)   = GRV;
    *L = double(R);
    //DEB(printf("toi: %lf to %d in:", R, *L, L));
    DEB(printf("tof: %lld to %lf in: %p\n", R, *L, L));
})

DEF_CMD(abs, FLT_CMD_BEG + 10, 2,{
    VAR(*L) = GLV;
    VAR(R)  = GRV;
    *L = abs(R);
    DEB(printf("abs: %lld to %lld in: %p\n", R, *L, L));
})

DEF_CMD(fabs, FLT_CMD_BEG + 11, 2,{
    FVAR(*L) = GFLV;
    FVAR(R)  = GFRV;
    *L = fabs(R);
    DEB(printf("fabs: %lf to %lf in: %p\n", R, *L, L));
})

DEF_CMD(cmpf, FLT_CMD_BEG + 12, 2,{
    FVAR(A) = GFRV;
    FVAR(B) = GFRV;

    CMPF(A, B)
    DEB(printf("cmpf: %lf and %lf", A, B));
})

#define STR_CMD_BEG FLT_CMD_BEG + 13
DEF_CMD(outs, STR_CMD_BEG, 1, {
    OUTS(GLV);
})

#define FILE_CMD_BEG STR_CMD_BEG + 1
DEF_CMD(readf, FILE_CMD_BEG, 2, {
    VAR(*FN) = GLV;
    VAR(*DST) = GLV;
    READ(FN, DST);
    DEB(printf("read file %s in %lld\n", FN, DST));
})

DEF_CMD(rbmp, FILE_CMD_BEG + 1, 2, {
    VAR(*FN) = GLV;
    VAR(*DST) = GLV;
    READ_BMP(FN, DST);
    DEB(printf("read bmp file %s\n in %p\n", FN, DST));
})

DEF_CMD(robj, FILE_CMD_BEG + 2, 5, {
    VAR(*FN) = GLV;
    VAR(*V)  = GLV;
    VAR(*VS) = GLV;
    VAR(*P)  = GLV;
    VAR(*PS) = GLV;
    READ_OBJ(FN, V, VS, P, PS);
    DEB(printf("read obj file %s\n in %p\n and %p\n", FN, V, P));
})

//#undef
//#undef
#undef STR_CMD_BEG
#undef FLT_CMD_BEG
#undef DRW_CMD_BEG
#undef FunctionF
#undef OperationF
#undef Operation