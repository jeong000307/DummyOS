#ifndef __ACPI_H__
#define __ACPI_H__

#include "AssemblyFunction.h"
#include "Error.h"
#include "Memory.h"
#include "String.h"
#include "SystemConfiguration.h"
#include "Type.h"

#pragma pack(push, 1)
typedef struct {
	UINT8  addressSpaceID;
	UINT8  registerBitWidth;
	UINT8  registerBitOffset;
	UINT8  accessSize;
	UINT64 address;
} GENERIC_ADDRESS_STRUCTURE;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	BYTE   signiture[4];
	UINT32 length;
	UINT8  revision;
	UINT8  checksum;
	BYTE   oemID[6];
	BYTE   oemTableID[8];
	UINT32 oemRevision;
	UINT32 creatorID;
	UINT32 creatorRevision;
} DESCRIPTION_HEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	DESCRIPTION_HEADER  header;

	ADDRESS				entry[1];
} EXTENDED_SYSTEM_DESCRIPTION_TABLE;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	DESCRIPTION_HEADER			header;

	UINT32						firmwareCtrl;
	UINT32						DSDT;
	UINT8						reserved;
	UINT8						preferredPMProfile;
	UINT16						SCIInterrupt;
	UINT32						SMICommandPort;
	UINT8						ACPIEnable;
	UINT8						ACPIDisable;
	UINT8						S4BIOSRequest;
	UINT8						PStateControl;
	UINT32						PM1aEventBlock;
	UINT32						PM1bEventBlock;
	UINT32						PM1aControlBlock;
	UINT32						PM1bControlBlock;
	UINT32						PM2ControlBlock;
	UINT32						PMTimerBlock;
	UINT32						GPE0Block;
	UINT32						GPE1Block;
	UINT8						PM1EventLength;
	UINT8						PM1ControlLength;
	UINT8						PM2ControlLength;
	UINT8						PMTimerLength;
	UINT8						GPE0Length;
	UINT8						GPE1Length;
	UINT8						GPE1Base;
	UINT8						CStateControl;
	UINT16						worstC2Latency;
	UINT16						worstC3Latency;
	UINT16						flushSize;
	UINT16						flushStride;
	UINT8						dutyOffset;
	UINT8						dutyWidth;
	UINT8						dayAlarm;
	UINT8						monthAlarm;
	UINT8						century;
	UINT16						bootArchitectureFlags;
	UINT8						reserved2;
	UINT32						flags;
	GENERIC_ADDRESS_STRUCTURE	resetRegister;
	UINT8						resetValue;
	UINT8						reserved3[3];
	UINT64						XFirmwareControl;
	UINT64						XDSDT;
	GENERIC_ADDRESS_STRUCTURE	XPM1aEventBlock;
	GENERIC_ADDRESS_STRUCTURE	XPM1bEventBlock;
	GENERIC_ADDRESS_STRUCTURE	XPM1aControlBlock;
	GENERIC_ADDRESS_STRUCTURE	XPM1bControlBlock;
	GENERIC_ADDRESS_STRUCTURE	XPM2ControlBlock;
	GENERIC_ADDRESS_STRUCTURE	XPMTimerBlock;
	GENERIC_ADDRESS_STRUCTURE	XGPE0Block;
	GENERIC_ADDRESS_STRUCTURE	XGPE1Block;
	GENERIC_ADDRESS_STRUCTURE	sleepControlRegister;
	GENERIC_ADDRESS_STRUCTURE	sleepStatusRegister;
	UINT64						hypervisorVendorIdentity;
} FIXED_ACPI_DESCRIPTION_TABLE;
#pragma pack(pop)

FIXED_ACPI_DESCRIPTION_TABLE*
DUMMYAPI GetFADT(
	VOID
);

STATUS
DUMMYAPI ParseACPITable(
	IN CONST	ROOT_SYSTEM_DESCRIPTOR_POINTER* _RSDP
);

STATIC STATUS
DUMMYAPI FindDescriptionTable(
	IN CONST	EXTENDED_SYSTEM_DESCRIPTION_TABLE*	_XSDT,
	IN CONST	BYTE*								_signature,
	OUT			FIXED_ACPI_DESCRIPTION_TABLE**		_FADT
);

STATIC BOOLEAN 
DUMMYAPI CheckRSDP(
	IN CONST	ROOT_SYSTEM_DESCRIPTOR_POINTER*	_RSDP
);

STATIC BOOLEAN 
DUMMYAPI CheckDescriptionHeader(
	IN CONST	DESCRIPTION_HEADER*	_header,
	IN CONST	BYTE*				_signature
);

#endif