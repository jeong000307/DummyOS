#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "AssemblyFunction.h"
#include "Descriptor.h"
#include "Error.h"
#include "Message.h"
#include "Timer.h"
#include "Type.h"

typedef enum {
    TimerInterruptIndex = 0x41
} INTERRUPT_VECTOR;

#pragma pack(push, 1)
typedef union {
	UINT16			data;

	struct {
		unsigned	interruptStackTable : 3;
		unsigned	: 5;
		unsigned	type : 4;
		unsigned	: 1;
		unsigned	descriptorPrivilegeLevel : 2;
		unsigned	present : 1;
	}				bits;
} INTERRUPT_DESCRIPTOR_ATTRIBUTE;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	UINT16							offsetLow;
	UINT16							segmentSelector;
	INTERRUPT_DESCRIPTOR_ATTRIBUTE	attribute;
	UINT16							offsetMiddle;
	UINT32							offsetHigh;
	UINT32							reserved;
} INTERRUPT_DESCRIPTOR;
#pragma pack(pop)

typedef struct {
	UINT64	rip;
	UINT64	cs;
	UINT64	rflags;
	UINT64	rsp;
	UINT64	ss;
} INTERRUPT_FRAME;

STATUS
DUMMYAPI InitializeInterrupt(
    VOID
);

VOID
DUMMYAPI CreateInterruptDescriptorAttribute(
	OUT			INTERRUPT_DESCRIPTOR_ATTRIBUTE* _descriptorAttribute,
	IN CONST	UINT16							_type,
	IN CONST	BYTE							_descriptorPrivilegeLevel,
	IN CONST	BOOLEAN							_present,
	IN CONST	BYTE							_interruptStackTable
);

VOID
DUMMYAPI SetIDTEntry(
	OUT			INTERRUPT_DESCRIPTOR*			_descriptor,
	IN CONST	INTERRUPT_DESCRIPTOR_ATTRIBUTE	_attribute,
	IN CONST	UINT64							_offset,
	IN CONST	UINT16							_segmentSelector
);

VOID
DUMMYAPI TimerInterruptHandler(
	IN	INTERRUPT_FRAME* _frame
);

#endif