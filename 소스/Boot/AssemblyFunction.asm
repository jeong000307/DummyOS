[BITS 64]

GLOBAL Inspect
GLOBAL Halt

[SECTION .text]

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