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
    cli
    mov rax, rcx
.rep:
    hlt
    jmp Test.rep
    ret

Pause:
    cli
.rep
    hlt
    jmp Pause.rep
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
    mov rax, InitializeSegmentRegister.next

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
    mov [rbx + 0x40], rax
    mov [rbx + 0x48], rbx
    mov [rbx + 0x50], rcx
    mov [rbx + 0x58], rdx
    mov [rbx + 0x60], rdi
    mov [rbx + 0x68], rsi

    lea rax, [rsp + 8]
    mov [rbx + 0x70], rax
    mov [rbx + 0x78], rbp

    mov [rbx + 0x80], r8
    mov [rbx + 0x88], r9
    mov [rbx + 0x90], r10
    mov [rbx + 0x98], r11
    mov [rbx + 0xa0], r12
    mov [rbx + 0xa8], r13
    mov [rbx + 0xb0], r14
    mov [rbx + 0xb8], r15

    mov rax, cr3
    mov [rbx + 0x00], rax
    mov rax, [rsp]
    mov [rbx + 0x08], rax
    pushfq
    pop qword [rbx + 0x10]

    mov ax, cs
    mov [rbx + 0x20], rax
    mov bx, ss
    mov [rbx + 0x28], rbx
    mov cx, fs
    mov [rbx + 0x30], rcx
    mov dx, gs
    mov [rbx + 0x38], rdx

    fxsave [rbx + 0xc0]

    push qword [rcx + 0x28]
    push qword [rcx + 0x70]
    push qword [rcx + 0x10]
    push qword [rcx + 0x20]
    push qword [rcx + 0x08]

    fxrstor [rcx + 0xc0]

    mov rax, [rcx + 0x00]
    mov cr3, rax
    mov rax, [rcx + 0x30]
    mov fs, ax
    mov rax, [rcx + 0x38]
    mov gs, ax

    mov rax, [rcx + 0x40]
    mov rbx, [rcx + 0x48]
    mov rcx, [rcx + 0x50]
    mov rdx, [rcx + 0x58]
    mov rdi, [rcx + 0x60]
    mov rsi, [rcx + 0x68]
    mov rbp, [rcx + 0x78]
    mov r8,  [rcx + 0x80]
    mov r9,  [rcx + 0x88]
    mov r10, [rcx + 0x90]
    mov r11, [rcx + 0x98]
    mov r12, [rcx + 0xa0]
    mov r13, [rcx + 0xa8]
    mov r14, [rcx + 0xb0]
    mov r15, [rcx + 0xb8]

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
    call TimerOnInterrupt
    call NotifyEndOfInterrupt
    iretq