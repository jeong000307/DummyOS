[BITS 64]

GLOBAL Halt

[SECTION .text]

Halt:
    hlt
    jmp Halt