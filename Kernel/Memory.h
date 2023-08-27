#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Type.h"
#include "Math.h"
#include "ErrorCode.h"
#include "AssemblyFunction.h"

typedef struct __MEMORY_MANAGER MEMORY_MANAGER;

typedef frame (*ALLOCATE)(
    IN MEMORY_MANAGER* this,
    IN size numberOfFrames);

typedef code (*FREE)(
    IN MEMORY_MANAGER* this,
    IN frame startFrame,
    IN size numberOfFrames);

typedef void (*MARK_ALLOCATED)(
    IN MEMORY_MANAGER* this,
    IN frame startFrame,
    IN size numberOfFrames);

struct MEMORY_MAP {
    uint64 bufferSize;
    uint64 mapSize;
    uint64 mapKey;
    uint64 descriptorSize;
    uint32 descriptorVersion;

    void*  buffer;
};

struct MemoryDescriptor {
    uint32  type;
    uint32  pad;
    uintptr physicalStart;
    uintptr virtualStart;
    size    numberOfPages;
    uint64  attribute;
};

enum MemoryType {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiMaxMemoryType
};

struct __MEMORY_MANAGER {
    frame                   beginFrame;
    frame                   endFrame;

    size                    numberOfPDPTables;
    size                    numberOfPageDirectories;

    uint64 _Alignas(KiB(4)) PML4Table[512];
    uint64 _Alignas(KiB(4)) PDPTable[512];
    uint64 _Alignas(KiB(4)) pageDirectory[64][512];
    bitmap                  allocationMap[1024 * 256];

    ALLOCATE                AllocateFrame; // _FRAME 추가
    FREE                    FreeFrame; // _FRAME 추가
    MARK_ALLOCATED          MarkAllocatedFrame; // _FRAME 추가
};

code InitializeMemoryManager(
  IN const struct MEMORY_MAP* memoryMap);

MEMORY_MANAGER* GetMemoryManager(void);

code SetMemory(
  void* destination,
  byte  value,
  size  length
);

code CopyMemory(
  void* source,
  void* destination,
  size  length
);

static bool IsUsableMemory(enum MemoryType memoryType);
static frame __AllocateFrame(MEMORY_MANAGER* this, size numberOfFrames);
static code  __FreeFrame(MEMORY_MANAGER* this, frame startFrame, size numberOfFrames);
static void __MarkAllocatedFrame(MEMORY_MANAGER* this, frame startFrame, size numberOfFrames);
static void SetBit(MEMORY_MANAGER* memoryManager, frame frame, bool allocated);
static bool GetBit(MEMORY_MANAGER* memoryManager, frame frame);

#endif