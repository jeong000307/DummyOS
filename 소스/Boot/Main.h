#ifndef __MAIN_H__
#define __MAIN_H__

#include "AssemblyFunction.h"
#include "EFI.h"
#include "PE.h"
#include "Type.h"

typedef struct _SYSTEM_CONFIGURATION  SYSTEM_CONFIGURATION;

typedef
VOID
(EFIAPI *ENTRY_POINT)  (
	IN CONST SYSTEM_CONFIGURATION* SystemConfiguration
);

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

struct _SYSTEM_CONFIGURATION {
	struct {
		UINT32                      HorizontalResolution;
		UINT32                      VerticalResolution;

		EFI_GRAPHICS_PIXEL_FORMAT   PixelFormat;
		EFI_PHYSICAL_ADDRESS        Framebuffer;
	} GraphicsInformation;

	struct {
		UINTN						MapSize;
		UINTN						MapKey;
		UINTN						DescriptorSize;
		UINT32						DescriptorVersion;

		EFI_MEMORY_DESCRIPTOR*		MemoryMap;
	} MemoryInformation;

	ROOT_SYSTEM_DESCRIPTOR_POINTER*	ACPIInformation;

	UINTN							NumberOfCore;
	EFI_PHYSICAL_ADDRESS			LocalAPICAddress;
	EFI_PHYSICAL_ADDRESS			IOAPICAddress;
};

EFI_STATUS
EFIAPI GetSystemConfiguration(
	VOID
);

EFI_STATUS
EFIAPI LoadKernel(
	VOID
);

EFI_STATUS
EFIAPI TransitToKernel(
	IN EFI_HANDLE _ImageHandle
);

STATIC VOID 
EFIAPI GetLoadAddressRange(
	IN VOID* _FileBuffer,
	OUT EFI_PHYSICAL_ADDRESS* _Start,
	OUT EFI_PHYSICAL_ADDRESS* _End);

STATIC VOID
EFIAPI LoadFileSegment(
	IN VOID*                _FileBuffer,
	IN EFI_PHYSICAL_ADDRESS	_FileStartAddress
);

#endif