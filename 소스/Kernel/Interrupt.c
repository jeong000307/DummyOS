#include "Interrupt.h"

STATIC  INTERRUPT_DESCRIPTOR    IDT[256];

STATUS
DUMMYAPI InitializeInterrupt(
	VOID
)   {
	INTERRUPT_DESCRIPTOR_ATTRIBUTE  interruptDescriptorAttribute;
	MESSAGE_QUEUE*                  messageQueue = GetMessageQueue();

	InitializeMessageQueue(messageQueue, 256);
	CreateInterruptDescriptorAttribute(&interruptDescriptorAttribute, interruptGate, 0, TRUE, 0);

	SetIDTEntry(&IDT[TimerInterruptIndex], interruptDescriptorAttribute, (ADDRESS)TimerInterruptHandler, GetCS());
	LoadIDT(sizeof(IDT) - 1, (ADDRESS)&IDT[0]);

	return SUCCESS;
}

VOID
DUMMYAPI CreateInterruptDescriptorAttribute(
	OUT			INTERRUPT_DESCRIPTOR_ATTRIBUTE* _descriptorAttribute,
	IN CONST	UINT16							_type,
	IN CONST	BYTE							_descriptorPrivilegeLevel,
	IN CONST	BOOLEAN							_present,
	IN CONST	BYTE							_interruptStackTable
) {
	_descriptorAttribute->bits.interruptStackTable = _interruptStackTable;
	_descriptorAttribute->bits.type = _type;
	_descriptorAttribute->bits.descriptorPrivilegeLevel = _descriptorPrivilegeLevel;
	_descriptorAttribute->bits.present = _present;

	return;
}

VOID
DUMMYAPI SetIDTEntry(
	OUT	        INTERRUPT_DESCRIPTOR*           _descriptor,
	IN CONST	INTERRUPT_DESCRIPTOR_ATTRIBUTE	_attribute,
	IN CONST	UINT64							_offset,
	IN CONST	UINT16							_segmentSelector
) {
	_descriptor->attribute = _attribute;
	_descriptor->offsetLow = _offset & 0xffffu;
	_descriptor->offsetMiddle = (_offset >> 16) & 0xffffu;
	_descriptor->offsetHigh = _offset >> 32;
	_descriptor->segmentSelector = _segmentSelector;
}