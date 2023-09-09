#pragma once

#include "Type.h"

struct FrameBufferConfiguration {
    uint32           horizontalResolution;
    uint32           verticalResolution;

    enum PixelFormat pixelFormat;
    byte* frameBuffer;
};

struct MemoryMap {
    uint64 bufferSize;
    uint64 mapSize;
    uint64 mapKey;
    uint64 descriptorSize;
    uint32 descriptorVersion;

    void* buffer;
};

#pragma pack(push, 1)
struct RSDP {
    byte   signiture[8];
    uint8  checksum;
    byte   oemID[6];
    uint8  revision;
    uint32 RSDTAddress;
    uint32 length;
    uint64 XSDTAddress;
    uint8  extendedChecksum;
    byte   reserved[3];
};
#pragma pack(pop)

struct SystemConfiguration {
  struct FrameBufferConfiguration frameBufferConfiguration;
  struct MemoryMap                memoryMap;
  struct RSDP                     RSDP;
  size                            numberOfCore;
  addr                            LocalAPICAddress;
  addr                            IOAPICAddress;
};

void SetSystemConfiguration(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap* memoryMap,
  const struct RSDP* RSDP);

struct SystemConfiguration* GetSystemConfiguration(void);