#ifndef __SYSTEM_CONFIGURATION_H__
#define __SYSTEM_CONFIGURATION_H__

#include "Error.h"
#include "Type.h"

typedef enum {
	pixelRedGreenBlueReserved8BitPerColor,
	pixelBlueGreenRedReserved8BitPerColor,
	pixelBitMask,
	pixelBltOnly,
	pixelFormatMax
} GRAPHICS_PIXEL_FORMAT;

typedef struct {
	UINT32                  horizontalResolution;
	UINT32                  verticalResolution;

	GRAPHICS_PIXEL_FORMAT   pixelFormat;
	ADDRESS                 framebuffer;
} GRAPHICS_INFORMATION;

typedef struct {
	UINT32  type;
	ADDRESS physicalStart;
	ADDRESS virtualStart;
	UINT64  numberOfPages;
	UINT64  attribute;
} MEMORY_DESCRIPTOR;

typedef struct {
	UINTN               mapSize;
	UINTN               mapKey;
	UINTN               descriptorSize;
	UINT32              descriptorVersion;

	MEMORY_DESCRIPTOR*	memoryMap;
} MEMORY_INFORMATION;

#pragma pack(push, 1)
typedef struct {
	BYTE   signiture[8];
	UINT8  checksum;
	BYTE   oemID[6];
	UINT8  revision;
	UINT32 RSDTAddress;
	UINT32 length;
	UINT64 XSDTAddress;
	UINT8  extendedChecksum;
	BYTE   reserved[3];
} ROOT_SYSTEM_DESCRIPTOR_POINTER;
#pragma pack(pop)

typedef struct {
	GRAPHICS_INFORMATION			graphicsInformation;
	MEMORY_INFORMATION				memoryInformation;
	ROOT_SYSTEM_DESCRIPTOR_POINTER*	ACPIInformation;
	UINTN							numberOfCore;
	ADDRESS							localAPICAddress;
	ADDRESS							IOAPICAddress;
} SYSTEM_CONFIGURATION;

STATUS
DUMMYAPI SetSystemConfiguration(
	IN CONST	SYSTEM_CONFIGURATION* _systemConfiguration
);

SYSTEM_CONFIGURATION*
DUMMYAPI GetSystemConfiguration(
	VOID
);

#endif