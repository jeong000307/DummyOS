[BITS 64]

GLOBAL Halt
GLOBAL IOOut32
GLOBAL IOIn32

[SECTION .text]

Halt:
    hlt
    jmp Halt

IOOut32:
    mov eax, edx
    mov dx, cx
    out dx, eax
    ret


IOIn32:
    mov dx, cx
    in eax, dx
    and rax, 0000FFFFh
    ret