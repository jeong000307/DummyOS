[BITS 64]

GLOBAL AP
GLOBAL Assert
GLOBAL Pause

[SECTION .text]

AP:
    mov rax, 0x101000
    mov rsp, 0x0
    mov rsi, 0x0
    mov rdi, 0x0
    jmp rax

Assert:
    cli
    mov rax, rcx
.rep:
    hlt
    jmp Assert.rep
    ret

Pause:
    hlt
    jmp Pause