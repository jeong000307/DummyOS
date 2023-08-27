[BITS 64]

GLOBAL Test
GLOBAL Pause

[SECTION .text]

Test:
    mov rax, rcx
    hlt
    jmp Test

Pause:
    hlt
    jmp Pause