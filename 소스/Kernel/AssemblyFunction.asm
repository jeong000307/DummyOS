[BITS 64]

EXTERN	systemStack
EXTERN	Main
EXTERN	TimerOnInterrupt

GLOBAL	Start
GLOBAL	Inspect
GLOBAL	Halt
GLOBAL	IOOut32
GLOBAL	IOIn32
GLOBAL	LoadGDT
GLOBAL	LoadIDT
GLOBAL	InitializeSegmentRegister
GLOBAL	GetCS
GLOBAL	GetSS
GLOBAL	GetCR3
GLOBAL	SetCR3
GLOBAL	SwitchContext
GLOBAL	SetInterruptFlag
GLOBAL	TimerInterruptHandler

[SECTION .text]

Start:
	CLI
	LEA	RSP, systemStack + 1024 * 1024
	STI
	JMP	Main

	RET

Inspect:
	CLI
	MOV RAX, RCX
	HLT
	JMP Inspect

	RET

Halt:
	CLI
	HLT
	JMP Halt

	RET

IOOut32:
	MOV EAX, EDX
	MOV DX, CX
	OUT DX, EAX

	RET

IOIn32:
	MOV DX, CX
	IN  EAX, DX

	RET

LoadGDT:
	PUSH    RBP
	MOV     RBP, RSP

	SUB     RSP, 10
	MOV     [RSP], CX
	MOV     [RSP + 0x2], RDX
	LGDT    [RSP]

	MOV		RSP, RBP
	POP		RBP
	
	RET

LoadIDT:
	PUSH	RBP
	MOV		RBP, RSP

	SUB		RSP, 10
	MOV		[RSP], CX
	MOV		[RSP + 0x2], RDX
	LIDT	[RSP]

	MOV		RSP, RBP
	POP		RBP

	RET

InitializeSegmentRegister:
	PUSH	RBP
	MOV		RBP, RSP

	MOV		DS, R8W
	MOV		ES, R8W
	MOV		FS, R8W
	MOV		GS, R8W
	MOV		SS, DX
	LEA		RAX, .Next

	PUSH	RCX
	PUSH	RAX

	DB		48h
	RETF

	.Next:
		MOV RSP, RBP
		POP RBP

		RET

GetCS:
	MOV	AX, CS

	RET

GetSS:
	MOV	AX, SS

	RET

GetCR3:
	MOV	RAX, CR3

	RET

SetCR3:
	MOV	CR3, RCX

	RET

SwitchContext:
	MOV		[RDX + 0x40], RAX
	MOV		[RDX + 0x48], RBX
	MOV		[RDX + 0x50], RCX
	MOV		[RDX + 0x58], RDX
	MOV		[RDX + 0x60], RDI
	MOV		[RDX + 0x68], RSI

	LEA		RAX, [RSP + 0x8]
	MOV		[RDX + 0x70], RAX
	MOV		[RDX + 0x78], RBP

	MOV		[RDX + 0x80], R8
	MOV		[RDX + 0x88], R9
	MOV		[RDX + 0x90], R10
	MOV		[RDX + 0x98], R11
	MOV		[RDX + 0xA0], R12
	MOV		[RDX + 0xA8], R13
	MOV		[RDX + 0xB0], R14
	MOV		[RDX + 0xB8], R15

	MOV		RAX, CR3
	MOV		[RDX + 0x00], RAX
	MOV		RAX, [RSP]
	MOV		[RDX + 0x08], RAX
	PUSHFQ
	POP		QWORD [RDX + 0x10]

	MOV		AX, CS
	MOV		[RDX + 0x20], RAX
	MOV		BX, SS
	MOV		[RDX + 0x28], RBX
	MOV		CX, FS
	MOV		[RDX + 0x30], RCX
	MOV		DX, GS
	MOV		[RDX + 0x38], RDX

	FXSAVE	[RDX + 0xC0]

	PUSH	QWORD [RCX + 0x28]
	PUSH	QWORD [RCX + 0x70]
	PUSH	QWORD [RCX + 0x10]
	PUSH	QWORD [RCX + 0x20]
	PUSH	QWORD [RCX + 0x08]

	FXRSTOR	[RCX + 0xC0]

	MOV		RAX, [RCX + 0x00]
	MOV		CR3, RAX
	MOV		RAX, [RCX + 0x30]
	MOV		FS, AX
	MOV		RAX, [RCX + 0x38]
	MOV		GS, AX

	MOV		RAX, [RCX + 0x40]
	MOV		RBX, [RCX + 0x48]
	MOV		RDX, [RCX + 0x58]
	MOV		RDI, [RCX + 0x60]
	MOV		RSI, [RCX + 0x68]
	MOV		RBP, [RCX + 0x78]
	MOV		R8,  [RCX + 0x80]
	MOV		R9,  [RCX + 0x88]
	MOV		R10, [RCX + 0x90]
	MOV		R11, [RCX + 0x98]
	MOV		R12, [RCX + 0xA0]
	MOV		R13, [RCX + 0xA8]
	MOV		R14, [RCX + 0xB0]
	MOV		R15, [RCX + 0xB8]

	MOV		RCX, [RCX + 0x50]

	IRET

SetInterruptFlag:
	AND	RCX, 1b
	CMP RCX, 1b
	JE	.Set
	JNE .Clear
	.Set:
		STI
	
		RET
	.Clear:
		CLI
	
		RET

NotifyEndOfInterrupt:
	MOV ECX, 0xFEE000B0
	MOV EDX, 0
	MOV [ECX], EDX

	RET

TimerInterruptHandler:
	JMP		TimerOnInterrupt
	JMP		NotifyEndOfInterrupt

	IRETQ