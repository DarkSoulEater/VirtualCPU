main:
    outs hello
    in BX

    mov AX 1
    mov SI 1

    call fact

    outs kStrAns
    out AX
    hlt

fact:
    cmp SI BX
    jge fact_end
        add SI 1
        mul AX SI
        call fact
    fact_end:
    ret

hello:
    db "Enter n > 0, for calculate n!"

kStrAns:
    db "Factorial n!:"