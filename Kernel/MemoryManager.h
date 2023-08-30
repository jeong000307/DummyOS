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
#define BYTE_PER_FRAME      KiB(4)
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
    uint64 mapSize;
    uint64 mapKey;
    uint64 descriptorSize;

    void*  buffer;
};

struct MemoryDescriptor {
    uint32  type;
    size    numberOfPages;
    uint64  attribute;

    addr    physicalStart;
    addr    virtualStart;
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
  enum MemoryType memoryType);

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

static void SetBit(
  MEMORY_MANAGER* memoryManager, 
  frame           frame, 
  bool            allocated);

static bool GetBit(
  MEMORY_MANAGER* memoryManager, 
  frame           frame);

code SetMemory(
  void* destination,
  byte  value,
  size  length);

code CopyMemory(
  void* source,
  void* destination,
  size  length);

#endif