#pragma once

#include "AssemblyFunction.h"
#include "Error.h"
#include "Type.h"

enum DescriptorType {
    upper8Bytes = 0,
    LDT = 2,
    TSSAvailable = 9,
    TSSBusy = 11,
    CallGate = 12,
    InterruptGate = 14,
    TrapGate = 15,

    ReadWrite = 2,
    ExecuteRead = 10
};

#pragma pack(push, 1)
union InterruptDescriptorAttribute {
    uint16 data;

    struct {
        uint16 interruptStackTable : 3;
        uint16 : 5;
        uint16 type : 4;
        uint16 : 1;
        uint16 descriptorPrivilegeLevel : 2;
        uint16 present : 1;
    } bits;
};
#pragma pack(pop)

#pragma pack(push, 1)
union SegmentDescriptor {
    uint64 data;
    struct {
        uint64 limitLow : 16;
        uint64 baseLow : 16;
        uint64 baseMiddle : 8;
        uint64 type : 4;
        uint64 systemSegment : 1;
        uint64 descriptorPrivilegeLvel : 2;
        uint64 present : 1;
        uint64 limitHigh : 4;
        uint64 available : 1;
        uint64 longMode : 1;
        uint64 defaultOperationSize : 1;
        uint64 granularity : 1;
        uint64 baseHigh : 8;
    } bits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct InterruptDescriptor {
    uint16 offsetLow;
    uint16 segmentSelector;
    union InterruptDescriptorAttribute attribute;
    uint16 offsetMiddle;
    uint32 offsetHigh;
    uint32 reserved;
};
#pragma pack(pop)

void SetCodeSegment(
    union SegmentDescriptor* descriptor,
    uint16                   type,
    uint64                   descriptorPrivilegeLevel,
    uint32                   base,
    uint32                   limit);

void SetDataSegment(
    union SegmentDescriptor* descriptor,
    uint16                   type,
    uint64                   descriptorPrivilegeLevel,
    uint32                   base,
    uint32                   limit);

union InterruptDescriptorAttribute MakeIDTAttribute(
    uint16 type,
    byte   descriptorPrivilegeLevel,
    bool   present,
    byte   interruptStackTable);

void SetIDTEntry(
    struct InterruptDescriptor* descriptor,
    union  InterruptDescriptorAttribute attribute,
    uint64 offset,
    uint16 segmentSelector);