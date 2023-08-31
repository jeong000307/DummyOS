#include "Interrupt.h"

static struct InterruptDescriptor IDT[256];
static MESSAGE_QUEUE messageQueue;

MESSAGE_QUEUE* GetMessageQueue(void) {
    return &messageQueue;
}

code InitializeInterrupt(void) {
    InitializeMessageQueue(&messageQueue, 256);

    SetIDTEntry(&IDT[TimerInterruptIndex], MakeIDTAttribute(InterruptGate, 0, true, 0), (addr)TimerInterruptHandler, 1 << 3);
    LoadIDT(sizeof(IDT) - 1, (addr)&IDT[0]);

    return SUCCESS;
}

union InterruptDescriptorAttribute MakeIDTAttribute(
  uint16 type,
  byte   descriptorPrivilegeLevel,
  bool   present,
  byte   interruptStackTable) {
    union InterruptDescriptorAttribute attribute = { 0 };

    attribute.bits.interruptStackTable = interruptStackTable;
    attribute.bits.type = type;
    attribute.bits.descriptorPrivilegeLevel = descriptorPrivilegeLevel;
    attribute.bits.present = present;

    return attribute;
}

void SetIDTEntry(
  struct InterruptDescriptor* descriptor,
  union  InterruptDescriptorAttribute attribute,
  uint64 offset,
  uint16 segmentSelector) {
    descriptor->attribute = attribute;
    descriptor->offsetLow = offset & 0xffffu;
    descriptor->offsetMiddle = (offset >> 16) & 0xffffu;
    descriptor->offsetHigh = offset >> 32;
    descriptor->segmentSelector = segmentSelector;
}