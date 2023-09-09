#include "Descriptor.h"

void SetCodeSegment(
  union SegmentDescriptor* descriptor,
  uint16                   type,
  uint64                   descriptorPrivilegeLevel,
  uint32                   base,
  uint32                   limit) {
    descriptor->data = 0;

    descriptor->bits.baseLow = base & 0xffffu;
    descriptor->bits.baseMiddle = (base >> 16) & 0xffu;
    descriptor->bits.baseHigh = (base >> 24) & 0xffu;

    descriptor->bits.limitLow = limit & 0xffffu;
    descriptor->bits.limitHigh = (limit >> 16) & 0xfu;

    descriptor->bits.type = type;
    descriptor->bits.systemSegment = 1;
    descriptor->bits.descriptorPrivilegeLvel = descriptorPrivilegeLevel;
    descriptor->bits.present = 1;
    descriptor->bits.available = 0;
    descriptor->bits.longMode = 1;
    descriptor->bits.defaultOperationSize = 0;
    descriptor->bits.granularity = 1;
}

void SetDataSegment(
  union SegmentDescriptor* descriptor,
  uint16                   type,
  uint64                   descriptorPrivilegeLevel,
  uint32                   base,
  uint32                   limit) {
    SetCodeSegment(descriptor, type, descriptorPrivilegeLevel, base, limit);
    descriptor->bits.longMode = 0;
    descriptor->bits.defaultOperationSize = 1;
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