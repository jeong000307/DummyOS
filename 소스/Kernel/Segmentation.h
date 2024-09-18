#ifndef __SEGMENTATION_H__
#define __SEGMENTATION_H__

#include "AssemblyFunction.h"
#include "Descriptor.h"
#include "Error.h"
#include "Type.h"

#pragma pack(push, 1)
typedef union {
	UINT64			data;
	struct {
		unsigned	limitLow : 16;
		unsigned	baseLow : 16;
		unsigned	baseMiddle : 8;
		unsigned	type : 4;
		unsigned	systemSegment : 1;
		unsigned	descriptorPrivilegeLevel : 2;
		unsigned	present : 1;
		unsigned	limitHigh : 4;
		unsigned	available : 1;
		unsigned	longMode : 1;
		unsigned	defaultOperationSize : 1;
		unsigned	granularity : 1;
		unsigned	baseHigh : 8;
	}				bits;
} SEGMENT_DESCRIPTOR;
#pragma pack(pop)

VOID
DUMMYAPI SetCodeSegment(
	OUT			SEGMENT_DESCRIPTOR* _descriptor,
	IN CONST	UINT16				_type,
	IN CONST	UINT32				_descriptorPrivilegeLevel,
	IN CONST	UINT32				_base,
	IN CONST	UINT32				_limit
);

VOID
DUMMYAPI SetDataSegment(
	OUT			SEGMENT_DESCRIPTOR* descriptor,
	IN CONST	UINT16				type,
	IN CONST	UINT32				descriptorPrivilegeLevel,
	IN CONST	UINT32				base,
	IN CONST	UINT32				limit
);

STATUS 
DUMMYAPI InitializeSegmentation(
	VOID
);

#endif