#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "AssemblyFunction.h"
#include "ErrorCode.h"
#include "Timer.h"
#include "Type.h"

static enum InterruptDescriptorType {
    upper8Bytes = 0,
    LDT = 2,
    TSSAvailable = 9,
    TSSBusy = 11,
    CallGate = 12,
    InterruptGate = 14,
    TrapGate = 15,
};

enum InterruptVector {
    TimerInterruptIndex = 0x41
};

union InterruptDescriptorAttribute {
    uint16 data;

    struct {
        uint16 interruptStackTable : 3;
        uint16 : 5;
        enum DescriptorType type : 4;
        uint16 : 1;
        uint16 descriptorPrivilegeLevel : 2;
        uint16 present : 1;
    } bits;
};

struct InterruptDescriptor {
    uint16 offsetLow;
    uint16 segmentSelector;
    union InterruptDescriptorAttribute attribute;
    uint16 offsetMiddle;
    uint16 offsetHigh;
    uint16 reserved;
};

struct InterruptFrame {
    uint64 rip;
    uint64 cs;
    uint64 rflags;
    uint64 rsp;
    uint64 ss;
};

code InitializeInterrupt(void);

union InterruptDescriptorAttribute MakeIDTAttribute(
  enum DescriptorType type, 
  uint8 descriptorPrivilegeLevel, 
  bool present, 
  uint8 interruptStackTable);

void SetIDTEntry(
    struct InterruptDescriptor* descriptor,
    union InterruptDescriptorAttribute attribute,
    uint64 offset,
    uint16 segmentSelector);

void TimerInterruptHandler(struct InterruptFrame* frame);

#endif