#include "Segmentation.h"

STATIC	SEGMENT_DESCRIPTOR	GDT[3];

VOID
DUMMYAPI SetCodeSegment(
    OUT			SEGMENT_DESCRIPTOR* _descriptor,
    IN CONST	UINT16				_type,
    IN CONST	UINT32				_descriptorPrivilegeLevel,
    IN CONST	UINT32				_base,
    IN CONST	UINT32				_limit
)   {
    _descriptor->data = 0;

    _descriptor->bits.baseLow = _base & 0xffffu;
    _descriptor->bits.baseMiddle = (_base >> 16) & 0xffu;
    _descriptor->bits.baseHigh = (_base >> 24) & 0xffu;

    _descriptor->bits.limitLow = _limit & 0xffffu;
    _descriptor->bits.limitHigh = (_limit >> 16) & 0xfu;

    _descriptor->bits.type = _type;
    _descriptor->bits.systemSegment = 1;
    _descriptor->bits.descriptorPrivilegeLevel = _descriptorPrivilegeLevel;
    _descriptor->bits.present = 1;
    _descriptor->bits.available = 0;
    _descriptor->bits.longMode = 1;
    _descriptor->bits.defaultOperationSize = 0;
    _descriptor->bits.granularity = 1;

    return;
}

VOID
DUMMYAPI SetDataSegment(
    OUT			SEGMENT_DESCRIPTOR* _descriptor,
    IN CONST	UINT16				_type,
    IN CONST	UINT32				_descriptorPrivilegeLevel,
    IN CONST	UINT32				_base,
    IN CONST	UINT32				_limit
)   {
    SetCodeSegment(_descriptor, _type, _descriptorPrivilegeLevel, _base, _limit);
    _descriptor->bits.longMode = 0;
    _descriptor->bits.defaultOperationSize = 1;

    return;
}

STATUS
DUMMYAPI InitializeSegmentation(
	VOID
)   {
	GDT[0].data = 0;
	SetCodeSegment(&GDT[1], executeRead, 0, 0, 0xfffff);
	SetDataSegment(&GDT[2], readWrite, 0, 0, 0xfffff);

	LoadGDT(sizeof(GDT) - 1, (ADDRESS)GDT);

	InitializeSegmentRegister(1 << 3, 2 << 3, 0);

	return SUCCESS;
}