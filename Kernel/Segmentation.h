#ifndef __SEGMENTATION_H__
#define __SEGMENTATION_H__

#include "AssemblyFunction.h"
#include "Error.h"
#include "Type.h"

enum SegmentDescriptorType {
    ReadWrite = 2,
    ExecuteRead = 10
};

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

static void SetCodeSegment(
  union SegmentDescriptor* descriptor, 
  enum DescriptorType      type,
  uint64                   descriptorPrivilegeLevel,
  uint32                   base,
  uint32                   limit);

static void SetDataSegment(
  union SegmentDescriptor* descriptor,
  enum DescriptorType      type,
  uint64                   descriptorPrivilegeLevel,
  uint32                   base,
  uint32                   limit);

code InitialzeSegmentation(void);

#endif
