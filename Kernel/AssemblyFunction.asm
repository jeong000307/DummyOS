[BITS 64]

extern systemStack
extern Main

GLOBAL Enter
GLOBAL Test
GLOBAL Pause
GLOBAL IOOut32
GLOBAL IOIn32
GLOBAL LoadGDT
GLOBAL LoadIDT
GLOBAL InitializeSegmentRegister
GLOBAL GetCS
GLOBAL SetCR3

[SECTION .text]

Enter:
    mov rsp, systemStack + 1024 * 1024
    call Main
    ret

Test:
    mov rax, rcx
    hlt
    jmp Test
    ret

Pause:
    hlt
    jmp Pause
    ret

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

LoadGDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], cx
    mov [rsp + 2], rdx
    lgdt [rsp]
    mov rsp, rbp
    pop rbp
    ret

LoadIDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], cx
    mov [rsp + 2], rdx
    lidt [rsp]
    mov rsp, rbp
    pop rbp
    ret

InitializeSegmentRegister:
    mov ds, r8w
    mov es, r8w
    mov fs, r8w
    mov gs, r8w

    push rbp
    mov rbp, rsp
    mov ss, dx
    mov rax, .next
    push rcx
    push rax
    o64 retf

.next:
    mov rsp, rbp
    pop rbp
    ret

GetCS:
    xor eax, eax
    mov ax, cs
    ret

SetCR3:
    mov cr3, rcx
    ret