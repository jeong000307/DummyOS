#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "AssemblyFunction.h"
#include "Error.h"
#include "Math.h"
#include "SystemConfiguration.h"
#include "Type.h"

#define MAX_CPU_CORE        16
#define EFI_PAGE_SIZE       KiB(4)
#define PDP_TABLE_SIZE      64
#define PAGE_DIRECTORY_SIZE 512
#define MAX_PHYSICAL_MEMORY GiB(PDP_TABLE_SIZE)
#define BYTES_PER_FRAME     KiB(4)
#define BITS_PER_LINE       (8 * sizeof(BITMAP))
#define NULL_FRAME          MAX_UINT64

typedef struct _MEMORY_MANAGER MEMORY_MANAGER;

typedef enum {
	efiReservedMemoryType,
	efiLoaderCode,
	efiLoaderData,
	efiBootServicesCode,
	efiBootServicesData,
	efiRuntimeServicesCode,
	efiRuntimeServicesData,
	efiConventionalMemory,
	efiUnusableMemory,
	efiACPIReclaimMemory,
	efiACPIMemoryNVS,
	efiMemoryMappedIO,
	efiMemoryMappedIOPortSpace,
	efiPalCode,
	efiMaxMemoryType
} MEMORY_TYPE;

struct _MEMORY_MANAGER {
	FRAME                           		beginFrame;
	FRAME                           		endFrame;

	__attribute__((aligned(4096)))	UINT64	PML4Table[512];
	__attribute__((aligned(4096)))	UINT64	PDPTable[512];
	__attribute__((aligned(4096)))	UINT64	pageDirectory[64][512];
	BITMAP                          		allocationMap[16 * 1024 * 1024];

	FRAME                           		(DUMMYAPI* AllocateFrame)(
												OUT			MEMORY_MANAGER* _this,
												IN CONST	UINTN			_numberOfFrames
											);
	STATUS									(DUMMYAPI* FreeFrame)(
												OUT			MEMORY_MANAGER* _this,
												IN CONST	FRAME			_startFrame,
												IN CONST	UINTN			_numberOfFrames
											);
	VOID									(DUMMYAPI* MarkAllocatedFrame)(
												OUT			MEMORY_MANAGER* _this,
												IN CONST	FRAME			_startFrame,
												IN CONST	UINTN			_numberOfFrames
											);
};

STATUS 
DUMMYAPI InitializeMemoryManager(
	IN CONST	MEMORY_INFORMATION*	_memoryInformation
);

MEMORY_MANAGER* 
DUMMYAPI GetMemoryManager(
	VOID
);

STATIC BOOLEAN 
DUMMYAPI IsUsableMemory(
	IN CONST	UINT32	_memoryType
);

STATIC FRAME 
DUMMYAPI _AllocateFrame(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	UINTN			_numberOfFrames
);

STATIC STATUS  
DUMMYAPI _FreeFrame(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	FRAME			_startFrame,
	IN CONST	UINTN			_numberOfFrames
);

STATIC VOID 
DUMMYAPI _MarkAllocatedFrame(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	FRAME			_startFrame,
	IN CONST	UINTN			_numberOfFrames
);

STATIC BOOLEAN 
DUMMYAPI GetBit(
	OUT			MEMORY_MANAGER*	_this, 
	IN CONST	FRAME			_frame
);

STATIC VOID 
DUMMYAPI SetBit(
	OUT			MEMORY_MANAGER*	_this, 
	IN CONST	FRAME			_frame, 
	IN CONST	BOOLEAN			_allocated
);

#endif