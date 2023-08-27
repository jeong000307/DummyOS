#include "Descriptor.h"

static union SegmentDescriptor GDT[3];

static void SetCodeSegment(
  union SegmentDescriptor* descriptor,
  enum DescriptorType type,
  uint64 descriptorPrivilegeLevel,
  uint32 base,
  uint32 limit) {
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

static void SetDataSegment(
  union SegmentDescriptor* descriptor,
  enum DescriptorType type,
  uint64 descriptorPrivilegeLevel,
  uint32 base,
  uint32 limit) {
    SetCodeSegment(descriptor, type, descriptorPrivilegeLevel, base, limit);
    descriptor->bits.longMode = 0;
    descriptor->bits.defaultOperationSize = 1;
}

code SetupSegments(void)    {
    GDT[0].data = 0;
    SetCodeSegment(&GDT[1], ExecuteRead, 0, 0, 0xfffff);
    SetDataSegment(&GDT[2], ReadWrite, 0, 0, 0xfffff);
    LoadGDT(sizeof(GDT) - 1, (uintptr)GDT);

    InitializeSegmentRegister(1 << 3, 2 << 3, 0);

    return SUCCESS;
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