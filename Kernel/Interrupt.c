#include "Interrupt.h"

extern void TimerOnInterrupt(void);

static struct InterruptDescriptor IDT[256];

code InitializeInterrupt(void) {
    SetIDTEntry(&IDT[TimerInterruptIndex], MakeIDTAttribute(InterruptGate, 0, true, 0), (uint64)TimerInterruptHandler, GetCS());
    LoadIDT(sizeof(IDT) - 1, (uint64)(&IDT[0]));

    return SUCCESS;
}

union InterruptDescriptorAttribute MakeIDTAttribute(
    enum DescriptorType type,
    uint8 descriptorPrivilegeLevel,
    bool present,
    uint8 interruptStackTable) {
    union InterruptDescriptorAttribute attribute = {0};

    attribute.bits.interruptStackTable = interruptStackTable;
    attribute.bits.type = type;
    attribute.bits.descriptorPrivilegeLevel = descriptorPrivilegeLevel;
    attribute.bits.present = present;

    return attribute;
}

void SetIDTEntry(
    struct InterruptDescriptor* descriptor,
    union InterruptDescriptorAttribute attribute,
    uint64 offset,
    uint16 segmentSelector) {
    descriptor->attribute = attribute;
    descriptor->offsetLow = offset & 0xffffu;
    descriptor->offsetMiddle = (offset >> 16) & 0xffffu;
    descriptor->offsetHigh = offset >> 32;
    descriptor->segmentSelector = segmentSelector;
}

void NotifyEndOfInterrupt(void) {
    volatile uint32* endOfInterrupt = (uint32 *)0xfee000b0;

    *endOfInterrupt = 0;
}

void TimerInterruptHandler(struct InterruptFrame* frame) {
    TimerOnInterrupt();
    NotifyEndOfInterrupt();
}