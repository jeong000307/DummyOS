#ifndef __MEMORYMANAGER_H__
#define __MEMORYMANAGER_H__

#include "Type.h"
#include "Math.h"
#include "Error.h"
#include "AssemblyFunction.h"

#define EFI_PAGE_SIZE       4096
#define PDP_TABLE_SIZE      64
#define PAGE_DIRECTORY_SIZE 512
#define MAX_PHYSICAL_MEMORY GiB(PDP_TABLE_SIZE)
#define BYTES_PER_FRAME     KiB(4)
#define BITS_PER_LINE       (8 * sizeof(bitmap))
#define NULL_FRAME          MAX_UINT64

typedef struct __MEMORY_MANAGER MEMORY_MANAGER;

typedef frame (*ALLOCATE_FRAME)(
  MEMORY_MANAGER* this,
  size            numberOfFrames);

typedef code (*FREE_FRAME)(
  MEMORY_MANAGER* this,
  frame           startFrame,
  size            numberOfFrames);

typedef void (*MARK_ALLOCATED_FRAME)(
  MEMORY_MANAGER* this,
  frame           startFrame,
  size            numberOfFrames);

struct MemoryMap {
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
    addr    physicalStart;
    addr    virtualStart;
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

    uint64 _Alignas(KiB(4)) PML4Table[512];
    uint64 _Alignas(KiB(4)) PDPTable[512];
    uint64 _Alignas(KiB(4)) pageDirectory[64][512];
    bitmap                  allocationMap[1024 * 256];

    ALLOCATE_FRAME          AllocateFrame;
    FREE_FRAME              FreeFrame;
    MARK_ALLOCATED_FRAME    MarkAllocatedFrame;
};

code InitializeMemoryManager(
  const struct MemoryMap* memoryMap);

MEMORY_MANAGER* GetMemoryManager(void);

static bool IsUsableMemory(
  uint32 memoryType);

static frame __AllocateFrame(
  MEMORY_MANAGER* this, 
  size            numberOfFrames);

static code  __FreeFrame(
  MEMORY_MANAGER* this, 
  frame           startFrame, 
  size            numberOfFrames);

static void __MarkAllocatedFrame(
  MEMORY_MANAGER* this, 
  frame           startFrame, 
  size            numberOfFrames);

static bool GetBit(MEMORY_MANAGER* this, frame frame);

static void SetBit(MEMORY_MANAGER* this, frame frame, bool allocated);

#endif