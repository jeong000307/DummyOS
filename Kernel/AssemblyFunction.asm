[BITS 64]

extern systemStack
extern Main
extern TimerOnInterrupt

GLOBAL Enter
GLOBAL Test
GLOBAL Pause
GLOBAL IOOut32
GLOBAL IOIn32
GLOBAL LoadGDT
GLOBAL LoadIDT
GLOBAL InitializeSegmentRegister
GLOBAL GetCS
GLOBAL GetSS
GLOBAL GetCR3
GLOBAL SetCR3
GLOBAL SwitchContext
GLOBAL ClearInterruptFlag
GLOBAL SetInterruptFlag
GLOBAL TimerInterruptHandler

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
    push rbp
    mov rbp, rsp

    mov ds, r8w
    mov es, r8w
    mov fs, r8w
    mov gs, r8w
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
    xor rax, rax
    mov ax, cs
    ret

GetSS:
    xor rax, rax
    mov ax, ss
    ret

GetCR3:
    mov rax, cr3
    ret

SetCR3:
    mov cr3, rcx
    ret

SwitchContext:
    mov [rsi + 0x40], rax
    mov [rsi + 0x48], rbx
    mov [rsi + 0x50], rcx
    mov [rsi + 0x58], rdx
    mov [rsi + 0x60], rdi
    mov [rsi + 0x68], rsi

    lea rax, [rsp + 8]
    mov [rsi + 0x70], rax
    mov [rsi + 0x78], rbp

    mov [rsi + 0x80], r8
    mov [rsi + 0x88], r9
    mov [rsi + 0x90], r10
    mov [rsi + 0x98], r11
    mov [rsi + 0xa0], r12
    mov [rsi + 0xa8], r13
    mov [rsi + 0xb0], r14
    mov [rsi + 0xb8], r15

    mov rax, cr3
    mov [rsi + 0x00], rax
    mov rax, [rsp]
    mov [rsi + 0x08], rax
    pushfq
    pop qword [rsi + 0x10]

    mov ax, cs
    mov [rsi + 0x20], rax
    mov bx, ss
    mov [rsi + 0x28], rbx
    mov cx, fs
    mov [rsi + 0x30], rcx
    mov dx, gs
    mov [rsi + 0x38], rdx

    fxsave [rsi + 0xc0]

    push qword [rdi + 0x28]
    push qword [rdi + 0x70]
    push qword [rdi + 0x10]
    push qword [rdi + 0x20]
    push qword [rdi + 0x08]

    fxrstor [rdi + 0xc0]

    mov rax, [rdi + 0x00]
    mov cr3, rax
    mov rax, [rdi + 0x30]
    mov fs, ax
    mov rax, [rdi + 0x38]
    mov gs, ax

    mov rax, [rdi + 0x40]
    mov rbx, [rdi + 0x48]
    mov rcx, [rdi + 0x50]
    mov rdx, [rdi + 0x58]
    mov rsi, [rdi + 0x68]
    mov rbp, [rdi + 0x78]
    mov r8,  [rdi + 0x80]
    mov r9,  [rdi + 0x88]
    mov r10, [rdi + 0x90]
    mov r11, [rdi + 0x98]
    mov r12, [rdi + 0xa0]
    mov r13, [rdi + 0xa8]
    mov r14, [rdi + 0xb0]
    mov r15, [rdi + 0xb8]

    mov rdi, [rdi + 0x60]

    o64 iret

ClearInterruptFlag:
    cli
    ret

SetInterruptFlag:
    sti
    ret

NotifyEndOfInterrupt:
    mov ecx, 0xfee000b0
    mov edx, 0
    mov [ecx], edx
    ret

TimerInterruptHandler:
    call NotifyEndOfInterrupt
    call TimerOnInterrupt
    iretq