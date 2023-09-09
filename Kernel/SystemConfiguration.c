#include "SystemConfiguration.h"

static struct SystemConfiguration systemConfiguration;

void SetSystemConfiguration(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap* memoryMap,
  const struct RSDP* RSDP) {
    systemConfiguration.frameBufferConfiguration = *frameBufferConfiguration;
    systemConfiguration.memoryMap = *memoryMap;
    systemConfiguration.RSDP = *RSDP;

    return;
}

struct SystemConfiguration* GetSystemConfiguration(void) {
    return &systemConfiguration;
}