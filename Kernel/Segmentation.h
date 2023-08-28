#ifndef __SEGMENTATION_H__
#define __SEGMENTATION_H__

#include "AssemblyFunction.h"
#include "ErrorCode.h"
#include "Type.h"

enum SegmentDescriptorType {
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
