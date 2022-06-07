; ---------------------------Main()-------------------------------------------- ;
main:
    cws Width Hieght

    mov fvar_RMD_path [kMyModelPath]
    mov fvar_RMD_vertices mem_MyModel_vertices
    mov fvar_RMD_vert_size mem_MyModel_vert_size
    mov fvar_RMD_index mem_MyModel_index
    mov fvar_RMD_ind_sz mem_MyModel_ind_sz
    call ReadModelData

    outs kStrModelRead
    outs kMyModelPath
    
    mov AX mem_MyModel_vert_size
    ;mov [AX] 10
    outs kStrIndexCount
    out [AX]
    mov AX mem_MyModel_ind_sz
    ;mov [AX] 10
    outs kStrVerticesCount
    out [AX]

    mov AX mem_MyModel_vertices
    mov CX mem_MyModel_vert_size
    mov CX [CX]
    mov DX mem_MyModel_global_vertices
    call CreateGlobalVertices

    mov AX mem_MyModel_global_vertices
    mov CX mem_MyModel_vert_size
    mov CX [CX]
    ;call OutModelVertices


    mov AX mem_MyModel_pivot
    call CreatePivot

    mov AX mem_MyModel_pivot
    mov BX mem_MyModel_vertices
    mov CX mem_MyModel_vert_size
    mov CX [CX]
    mov DX mem_MyModel_global_vertices
    mov YMM0 1.1344640138
    ;mov YMM0 0.01745329252
    call RotateYModel

    mov AX mem_MainCamera_screen_dist
    mov [AX] 10.0
    

    mov AX mem_MainCamera_pivot
    call CreatePivot
    
    mov AX mem_MainCamera_pivot
    mov DX Vector3
    mov [DX] 0.0
    mov [DX + 8] 150.0
    mov [DX + 16] -1100.0
    call MovePivot


    mov AX mem_MyModel_index
    mov CX mem_MyModel_ind_sz
    mov CX [CX]
    mov DX mem_MyModel_global_vertices
    call RenderModel

    draw

    hlt

; ---------------------------(Main)--------------------------------------------- ;


; Class Model: MyModel

kMyModelPath:
    db "CPU/src/graphics/model/alduin_.obj"
    ;db "src/graphics/model/my_model.obj"

mem_MyModel_vertices:
    50000 ; limit 15000 vertices
mem_MyModel_vert_size:
    410000
mem_MyModel_index:
    410008 ; limimt 20000 polygons
mem_MyModel_ind_sz:
    890008
mem_MyModel_pivot:
    890016
mem_MyModel_global_vertices:
    890112
; ends at 1250112 


; Class Camera: MainCamera
mem_MainCamera_pivot:
    1250112
mem_MainCamera_screen_dist:
    1250208
; ends at 1250216

; --------------------Read_Model_Data()----------------------------------------- ;

ReadModelData:      ; DX : model_path
    mov AX fvar_RMD_vertices
    mov BX fvar_RMD_vert_size
    mov CX fvar_RMD_index
    mov DX fvar_RMD_ind_sz
    mov R8 fvar_RMD_path
    robj [R8] [AX] [BX] [CX] [DX]
    ret


fvar_RMD_path:
    0
fvar_RMD_vertices:
    0
fvar_RMD_vert_size:
    0
fvar_RMD_index:
    0
fvar_RMD_ind_sz:
    0

; --------------------(Read_Model_Data)------------------------------------------ ;



; ---------------------Create_Global_Vertices()----------------------------------- ;

CreateGlobalVertices:     ; vertices : AX, vert_size : CX, global_vertices : DX 
    mov [DX] [AX]
    add AX 8
    add DX 8
    mov [DX] [AX]
    add AX 8
    add DX 8
    mov [DX] [AX]
    add AX 8
    add DX 8
    loop CreateGlobalVertices
    ret

; ---------------------(Create_Global_Vertices)------------------------------------ ;




; ------------------------------Create_Pivot()------------------------------------- ;

CreatePivot:     ; pivot : AX
    mov [AX    ]   0.0
    mov [AX + 8]   0.0
    mov [AX + 16]  0.0

    mov [AX + 24]  1.0
    mov [AX + 32]  0.0
    mov [AX + 40]  0.0

    mov [AX + 48]  0.0
    mov [AX + 56]  1.0 
    mov [AX + 64]  0.0

    mov [AX + 72]  0.0
    mov [AX + 80]  0.0
    mov [AX + 88]  1.0
    ret

; ------------------------------(Create_Pivot)------------------------------------- ;




; -------------------------Move_Pivot()-------------------------------------------- ;

MovePivot:          ; pivot : AX, vec3 : DX
    call AddVector3
    ret

; -------------------------(Move_Pivot)-------------------------------------------- ;




; -------------------------Rotate_Pivot()----------------------------------------- ;

RotateXPivot:          ; pivot : AX, angle : YMM0
    add AX 24
    call RotateXVector3
    add AX 24
    call RotateXVector3
    add AX 24
    call RotateXVector3
    ret


RotateYPivot:          ; pivot : AX, angle : YMM0
    add AX 24
    call RotateYVector3
    add AX 24
    call RotateYVector3
    add AX 24
    call RotateYVector3
    ret


RotateZPivot:          ; pivot : AX, angle : YMM0
    add AX 24
    call RotateZVector3
    add AX 24
    call RotateZVector3
    add AX 24
    call RotateZVector3    
    ret

; ------------------------(Rotate_X_Pivot)------------------------------------------ ;




; --------------------------Rotate_Vector3()----------------------------------------- ;

RotateXVector3:     ; AX : vec, YMM0 : angle
    cosf YMM1 YMM0
    sinf YMM2 YMM0

    mov  YMM3 [AX + 8]
    mulf YMM3 YMM1
    mov  YMM4 [AX + 16]
    mulf YMM4 YMM2
    subf YMM3 YMM4

    mov  YMM5 [AX + 8]
    mulf YMM5 YMM2
    mov  YMM6 [AX + 16]
    mulf YMM6 YMM1
    addf YMM5 YMM6

    mov [AX + 8]  YMM3
    mov [AX + 16] YMM5
    ret


RotateYVector3:     ; AX : vec, YMM0 : angle
    cosf YMM1 YMM0
    sinf YMM2 YMM0

    mov  YMM3 [AX]
    mulf YMM3 YMM1
    mov  YMM4 [AX + 16]
    mulf YMM4 YMM2
    subf YMM3 YMM4

    mov  YMM5 [AX]
    mulf YMM5 YMM2
    mov  YMM6 [AX + 16]
    mulf YMM6 YMM1
    addf YMM5 YMM6

    mov [AX    ]  YMM3
    mov [AX + 16] YMM5
    ret


RotateZVector3:     ; AX : vec, YMM0 : angle
    cosf YMM1 YMM0
    sinf YMM2 YMM0

    mov  YMM3 [AX]
    mulf YMM3 YMM1
    mov  YMM4 [AX + 8]
    mulf YMM4 YMM2
    subf YMM3 YMM4

    mov  YMM5 [AX]
    mulf YMM5 YMM2
    mov  YMM6 [AX + 8]
    mulf YMM6 YMM1
    addf YMM5 YMM6

    mov [AX    ] YMM3
    mov [AX + 8] YMM5
    ret

; ---------------------------(Rotate_Vector3)----------------------------------------- ;




; -----------------------------Сhange_Vector3()--------------------------------------- ;

AddVector3:         ; AX : vec1, DX : vec2
    addf [AX] [DX]
    add AX 8
    add DX 8
    addf [AX] [DX]
    add AX 8
    add DX 8
    addf [AX] [DX]
    ret


SubVector3:         ; AX : vec1, DX : vec2
    subf [AX] [DX]
    add AX 8
    add DX 8
    subf [AX] [DX]
    add AX 8
    add DX 8
    subf [AX] [DX]
    ret


MulVector3:         ; AX : vec1, DX : k
    mulf [AX] DX
    add AX 8
    mulf [AX] DX
    add AX 8
    mulf [AX] DX
    ret    


DivVector3:         ; AX : vec1, DX : k
    divf [AX] DX
    add AX 8
    divf [AX] DX
    add AX 8
    divf [AX] DX
    ret   


CopyVector3:        ; AX : dst_vec, DX : src_vec
    mov [AX] [DX]
    add AX 8
    add DX 8
    mov [AX] [DX]
    add AX 8
    add DX 8
    mov [AX] [DX]
    ret

; -----------------------------(Сhange_Vector3)---------------------------------------- ;




; ------------------------------Base_Change--------------------------------------------- ;

ToLocalCoords:      ; pivot : AX, vec : DX
    push AX
    push DX
    mov BX AX
    mov AX DX
    mov DX BX
    call SubVector3

    pop DX
    pop AX 


    mov  BX [AX + 24]
    mulf BX [DX]
    push BX
    mov  BX [AX + 48]
    mulf BX [DX + 8 ]
    push BX
    mov  BX [AX + 72]
    mulf BX [DX + 16]

    mov  YMM0 BX
    pop  BX
    addf YMM0 BX
    pop  BX
    addf YMM0 BX


    mov  BX [AX + 32]
    mulf BX [DX]
    push BX
    mov  BX [AX + 56]
    mulf BX [DX + 8]
    push BX
    mov  BX [AX + 80]
    mulf BX [DX + 16]

    mov  YMM1 BX
    pop  BX
    addf YMM1 BX
    pop  BX
    addf YMM1 BX


    mov  BX [AX + 40]
    mulf BX [DX]
    push BX
    mov  BX [AX + 64]
    mulf BX [DX + 8]
    push BX
    mov  BX [AX + 88]
    mulf BX [DX + 16]

    mov  YMM2 BX
    pop  BX
    addf YMM2 BX
    pop  BX
    addf YMM2 BX


    mov [DX     ] YMM0
    mov [DX + 8 ] YMM1
    mov [DX + 16] YMM2   
    ret


ToGlobalCoords:      ; pivot : AX, vec : DX
    mov  BX [AX + 24]
    mulf BX [DX]
    push BX
    mov  BX [AX + 32]
    mulf BX [DX + 8 ]
    push BX
    mov  BX [AX + 40]
    mulf BX [DX + 16]

    mov  YMM0 BX
    pop  BX
    addf YMM0 BX
    pop  BX
    addf YMM0 BX


    mov  BX [AX + 48]
    mulf BX [DX]
    push BX
    mov  BX [AX + 56]
    mulf BX [DX + 8]
    push BX
    mov  BX [AX + 64]
    mulf BX [DX + 16]

    mov  YMM1 BX
    pop  BX
    addf YMM1 BX
    pop  BX
    addf YMM1 BX


    mov  BX [AX + 72]
    mulf BX [DX]
    push BX
    mov  BX [AX + 80]
    mulf BX [DX + 8]
    push BX
    mov  BX [AX + 88]
    mulf BX [DX + 16]

    mov  YMM2 BX
    pop  BX
    addf YMM2 BX
    pop  BX
    addf YMM2 BX


    mov [DX     ] YMM0
    mov [DX + 8 ] YMM1
    mov [DX + 16] YMM2

    mov BX AX
    mov AX DX
    mov DX BX
    call AddVector3
    ret

; ------------------------------(Base_Change)------------------------------------------- ;




; ------------------------------Matrix_Arithmetic---------------------------------------- ;

MulMatrix3x3_Vector3:
    outs kStrUndefined
    ret

; -----------------------------(Matrix_Arithmetic)--------------------------------------- ;




; --------------------------------TransformModel------------------------------------------ ;

MoveModel:          ; pivot : AX, global_vertices : BX, vert_size : CX, vec : DX 
    push DX
    call MovePivot
    pop DX

    MoveModel_:
        push DX
        mov AX BX
        call AddVector3
        add BX 24
        pop DX
        loop MoveModel_

    ret


RotateXModel:       ; pivot : AX, local_vertices : BX, vert_size : CX, global_vertices : DX, angle : YMM0
    push AX
    call RotateXPivot
    pop AX

    RotateXModel_:
        push AX
        push DX
        mov AX DX
        mov DX BX
        call CopyVector3
        pop DX
        pop AX

        push AX
        push BX
        push DX
        call ToGlobalCoords
        pop DX
        pop BX
        pop AX

        add BX 24
        add DX 24
        loop RotateXModel_

    ret


RotateYModel:       ; pivot : AX, local_vertices : BX, vert_size : CX, global_vertices : DX, angle : YMM0
    push AX
    call RotateYPivot
    pop AX

    RotateYModel_:
        push AX
        push DX
        mov AX DX
        mov DX BX
        call CopyVector3
        pop DX
        pop AX

        push AX
        push BX
        push DX
        call ToGlobalCoords
        pop DX
        pop BX
        pop AX

        add BX 24
        add DX 24
        loop RotateYModel_
        
    ret


RotateZModel:       ; pivot : AX, local_vertices : BX, vert_size : CX, global_vertices : DX, angle : YMM0
    push AX
    call RotateZPivot
    pop AX

    RotateZModel_:
        push AX
        push DX
        mov AX DX
        mov DX BX
        call CopyVector3
        pop DX
        pop AX

        push AX
        push BX
        push DX
        call ToGlobalCoords
        pop DX
        pop BX
        pop AX

        add BX 24
        add DX 24
        loop RotateZModel_
        
    ret


ScaleModel:         ; model : AX, k : DX

    ret

; -------------------------------(TransformModel)------------------------------------------ ;




; ----------------------------------OutModelData------------------------------------------  ;

OutModelVertices:       ; vertices : AX, vert_size : CX 
    outs kStrVerticesSize
    out CX

    OutModelVertices_:
        outf [AX     ]
        outf [AX + 8 ]
        outf [AX + 16]
        outs kStrEndl
        add AX 24
        loop OutModelVertices_

    ret

kStrVerticesSize:
    db "Vertices count:"


OutModelPivot:
    outs kStrUndefined
    ret

; ---------------------------------(OutModelData)-----------------------------------------  ;




; ----------------------------------CameraMethods------------------------------------------- ;

ScreenProection:        ; pivot : AX, screen_dist : BX, vec : DX
    ;outs kStrEndl
    ;outf [BX]
    ;outf [DX]
    ;outf [DX + 8]
    ;outf [DX + 16] 
    mov rvar_ScreenProectionF 1

    push AX
    push BX
    push DX
    call ToLocalCoords
    pop DX
    pop BX
    pop AX

    ;outs kStrEndl
    ;outf [DX]
    ;outf [DX + 8]
    ;outf [DX + 16]
    ;outs kStrEndl

    ;outs kStrEndl
    ;outf [BX]
    ;outs kStrEndl

    cmpf [DX + 16] [BX]
    jl SP_Skeep_

    mov  YMM1 [BX]
    divf YMM1 [DX + 16]

    tof  YMM2 kScreenWidth
    mov  YMM3 [BX]
    mulf YMM3 2.0
    ;outf [BX]

    sinf YMM4 1.0471975512 ; observe range
    cosf YMM5 1.0471975512 ; observe range
    divf YMM4 YMM5
    ;outf YMM4

    mulf YMM3 YMM4
    divf YMM2 YMM3
    mulf YMM1 YMM2

    ;outf YMM3
    ;outf YMM2
    ;outf YMM1

    mov  rvar_ScreenProectionX [DX]
    mulf rvar_ScreenProectionX YMM1
    mov  rvar_ScreenProectionY [DX + 8]
    mulf rvar_ScreenProectionY YMM1

    toi rvar_ScreenProectionX rvar_ScreenProectionX
    toi rvar_ScreenProectionY rvar_ScreenProectionY

    mov R8 kScreenWidth
    div R8 2
    add rvar_ScreenProectionX R8

    mov R8 kScreenHieght
    div R8 2
    sub rvar_ScreenProectionY R8
    mul rvar_ScreenProectionY -1

    mov [DX     ] rvar_ScreenProectionX
    mov [DX + 8 ] rvar_ScreenProectionY
    mov [DX + 16] rvar_ScreenProectionF

    cmp rvar_ScreenProectionX 0
    jl SP_Skeep_

    cmp rvar_ScreenProectionY 0
    jl SP_Skeep_

    cmp rvar_ScreenProectionX kScreenWidth
    jge SP_Skeep_

    cmp rvar_ScreenProectionY kScreenHieght
    jge SP_Skeep_

    SP_Skeep_:
        mov rvar_ScreenProectionF 0
        ret
    ret

rvar_ScreenProectionX:
    0
rvar_ScreenProectionY:
    0
rvar_ScreenProectionF:
    0
; ----------------------------------(CameraMethods)------------------------------------------ ;




; -----------------------------Render_Model()------------------------------------------------ ;

RenderModel:        ; index : AX, ind_size : CX, global_vertices : DX
    mov fvar_DIP_ind1 [AX     ]
    mov fvar_DIP_ind2 [AX + 8 ]
    mov fvar_DIP_ind3 [AX + 16]


    add AX 24
    push AX
    push CX
    push DX
    call DrawIndexPolygon
    pop DX
    pop CX
    pop AX
    loop RenderModel

; ------------------------------(Render_Model)------------------------------------------------ ;




; -----------------------------Draw_Index_Polygon()-------------------------------------------- ;

DrawIndexPolygon:   ; global_vertices : DX
    push DX
    mov AX fvar_DIP_ind1
    mul AX 24
    add DX AX
    mov AX mem_DP_Vector3_1
    call CopyVector3
    pop DX

    push DX
    mov AX fvar_DIP_ind2
    mul AX 24
    add DX AX
    mov AX mem_DP_Vector3_2
    call CopyVector3
    pop DX

    mov AX fvar_DIP_ind3
    mul AX 24
    add DX AX
    mov AX mem_DP_Vector3_3
    call CopyVector3


    call DrawPolygon
    ret


fvar_DIP_ind1:
    0
fvar_DIP_ind2:
    0
fvar_DIP_ind3:
    0

; -----------------------------(Draw_Index_Polygon)-------------------------------------------- ;




; -------------------------------Draw_Polygon()------------------------------------------------- ;

DrawPolygon:
    mov AX mem_MainCamera_pivot
    mov BX mem_MainCamera_screen_dist
    mov DX mem_DP_Vector3_1
    call ScreenProection


    mov AX mem_MainCamera_pivot
    mov BX mem_MainCamera_screen_dist
    mov DX mem_DP_Vector3_2
    call ScreenProection


    mov AX mem_MainCamera_pivot
    mov BX mem_MainCamera_screen_dist
    mov DX mem_DP_Vector3_3
    call ScreenProection


    mov AX mem_DP_Vector3_1
    mov BX mem_DP_Vector3_2
    mov CX mem_DP_Vector3_3
    call DrawTriangle
    ret

mem_DP_Vector3_1:
    1250216
mem_DP_Vector3_2:
    1250240
mem_DP_Vector3_3:
    1250264

; -------------------------------(Draw_Polygon)------------------------------------------------- ;




; ----------------------------------------------------------------------------- ;

DrawTriangle:       ; PtrPoint1 : AX, PtrPoint2 : BX, PtrPoint3 : CX
    push AX
    push BX
    push CX

    push [AX    ]
    push [AX + 8]
    push [BX    ]
    push [BX + 8]
    mov BX ColorWhite
    call DrawLine

    pop CX 
    pop BX 
    pop AX
    push AX
    push BX
    push CX

    push [BX    ]
    push [BX + 8]
    push [CX    ]
    push [CX + 8]
    mov BX ColorWhite
    call DrawLine

    pop CX 
    pop BX 
    pop AX

    push [CX    ]
    push [CX + 8]
    push [AX    ]
    push [AX + 8]
    mov BX ColorWhite
    call DrawLine

    ret

; ----------------------------------------------------------------------------- ;




; ----------------------------------------------------------------------------- ;

DrawLine:       ; X0,Y0,X1,Y1 : S, COLOR: BX 
    pop R15 ; ret
    pop R11 ; Y1
    pop R10 ; X1
    pop R9  ; Y0
    pop R8  ; X0
    push R15


    push BX
    mov BX 0
    mov DX kScreenWidth

    mov AX R8
    call DL_CheckCoords
    mov R8 AX

    mov AX R10
    call DL_CheckCoords
    mov R10 AX

    mov DX kScreenHieght
    mov AX R9
    call DL_CheckCoords
    mov R9 AX

    mov AX R11
    call DL_CheckCoords
    mov R11 AX
    pop BX

    mov AX R10
    sub AX R8
    abs SI AX

    mov DX R11
    sub DX R9
    abs DI DX

    cmp SI DI
    jle DrawLine_DrawY
        ; DrawX
        cmp R8 R10 
        jle DrawLine_SKIP_SWAPX1X0 ; (X0 > X1)
            mov R15 R10
            mov R10 R8
            mov R8 R15

            mov R15 R11
            mov R11 R9
            mov R9 R15
        DrawLine_SKIP_SWAPX1X0:

        tof YMM0 R8  ; X0
        tof YMM1 R9  ; Y0
        tof YMM2 R10 ; X1
        tof YMM3 R11 ; Y1
    
        subf YMM3 YMM1 ; Y1 - Y0
        subf YMM2 YMM0 ; X1 - X0
        divf YMM3 YMM2 ; a = (Y1 - Y0) / (X1 - X0)


        mov CX SI
        add CX 1
        DrawLine_DrawPixelX:
            toi AX YMM1 
            toi DX YMM0 
            call DrawPixel
            addf YMM0 1.0
            addf YMM1 YMM3
            loop DrawLine_DrawPixelX

        jmp DrawLine_END
    DrawLine_DrawY: ; DrawY
        cmp R9 R11
        jle DrawLine_SKIP_SWAPY1Y0 ; (Y0 > Y1)
            mov R15 R10
            mov R10 R8
            mov R8 R15

            mov R15 R11
            mov R11 R9
            mov R9 R15
        DrawLine_SKIP_SWAPY1Y0:

        tof YMM0 R8  ; X0
        tof YMM1 R9  ; Y0
        tof YMM2 R10 ; X1
        tof YMM3 R11 ; Y1

        subf YMM3 YMM1 ; Y1 - Y0
        subf YMM2 YMM0 ; X1 - X0
        divf YMM2 YMM3 ; a = (X1 - X0) / (Y1 - Y0)

        mov CX DI
        add CX 1
        DrawLine_DrawPixelY:
            toi AX YMM1 
            toi DX YMM0 
            call DrawPixel
            addf YMM1 1.0
            addf YMM0 YMM2
            loop DrawLine_DrawPixelY

    DrawLine_END:
    ret

DL_CheckCoords: ; coords : AX, L : BX, R : DX
    cmp AX BX
    jge DL_L_
        mov AX BX
    DL_L_:
    
    cmp AX DX
    jle DL_R_
        mov AX DX
    DL_R_:
    ret

; ----------------------------------------------------------------------------- ;




; ----------------------------------------------------------------------------- ;

DrawPixel:      ; X : DX, Y : AX, COLOR : BX
    cmp DX 0
    jl DP_END_

    cmp DX kScreenWidth
    jge DP_END_

    cmp AX 0
    jl DP_END_

    cmp AX kScreenHieght
    jge DP_END_

    mul AX Width
    add AX DX
    mul AX 4

    cmp AX 0
    jl DP_END_

    mov R10 kScreenWidth
    mul R10 kScreenHieght
    mul R10 24
    cmp AX R10
    jge DP_END_

    add AX VS
    set [AX] BX
    ;draw

    DP_END_:
    ret

; ----------------------------------------------------------------------------- ;


ColorRed:
    4278190335
ColorGreen:
    4278255360
ColorBlue:
    4294901760
ColorWhite:
    4294967295
Width:
    1920
Hieght:
    1080

kScreenWidth:
    1920
kScreenHieght:
    1080

kStrEndl:
    db ""

kStrModelRead:
    db "Model read from:"

kStrIndexCount:
    db "Indexs count:"

kStrVerticesCount:
    db "Vertices count:"

kStrUndefined:
    db "Undefined"

Vector3:
    1250288
