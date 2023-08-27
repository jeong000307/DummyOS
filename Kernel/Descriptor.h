#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#pragma pack(1)

#include "AssemblyFunction.h"
#include "ErrorCode.h"
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

union SegmentDescriptor {
    uint64 data;
    struct {
        unsigned limitLow : 16;
        unsigned baseLow : 16;
        unsigned baseMiddle : 8;
        enum DescriptorType type : 4;
        unsigned systemSegment : 1;
        unsigned descriptorPrivilegeLvel : 2;
        unsigned present : 1;
        unsigned limitHigh : 4;
        unsigned available : 1;
        unsigned longMode : 1;
        unsigned defaultOperationSize : 1;
        unsigned granularity : 1;
        unsigned baseHigh : 8;
    } bits;
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

static void SetCodeSegment(
  union SegmentDescriptor* descriptor, 
  enum DescriptorType type,
  uint64 descriptorPrivilegeLevel,
  uint32 base,
  uint32 limit);

static void SetDataSegment(
  union SegmentDescriptor* descriptor,
  enum DescriptorType type,
  uint64 descriptorPrivilegeLevel,
  uint32 base,
  uint32 limit);

code SetupSegments(void);

void SetIDTEntry(
    struct InterruptDescriptor* descriptor,
    union InterruptDescriptorAttribute attribute,
    uint64 offset,
    uint16 segmentSelector);

#endif
