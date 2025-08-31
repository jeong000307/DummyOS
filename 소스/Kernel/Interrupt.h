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

typedef union {
	UINT16						data;

	struct {
		UINT16					interruptStackTable : 3;
		UINT16					: 5;
		UINT16					type : 4;
		UINT16					: 1;
		UINT16					descriptorPrivilegeLevel : 2;
		UINT16					present : 1;
	} __attribute__((packed)) 	bits;
} __attribute__((packed)) INTERRUPT_DESCRIPTOR_ATTRIBUTE;

typedef struct {
	UINT16							offsetLow;
	UINT16							segmentSelector;
	INTERRUPT_DESCRIPTOR_ATTRIBUTE	attribute;
	UINT16							offsetMiddle;
	UINT32							offsetHigh;
	UINT32							reserved;
} __attribute__((packed)) INTERRUPT_DESCRIPTOR;

typedef struct {
	UINT64	rip;
	UINT64	cs;
	UINT64	rflags;
	UINT64	rsp;
	UINT64	ss;
} __attribute__((packed)) INTERRUPT_FRAME;

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