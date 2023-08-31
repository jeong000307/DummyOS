#ifndef __MAIN_H__
#define __MAIN_H__

#include "ACPI.h"
#include "AssemblyFunction.h"
#include "Console.h"
#include "DataStructure.h"
#include "Error.h"
#include "Graphics.h"
#include "Heap.h"
#include "Interrupt.h"
#include "Memory.h"
#include "MemoryManager.h"
#include "PCI.h"
#include "Segmentation.h"
#include "Task.h"
#include "Timer.h"
#include "Type.h"

struct SystemConfiguration {
    struct FrameBufferConfiguration frameBufferConfiguration;
    struct MemoryMap                memoryMap;
    struct XSDP                     XSDP;
};

void SetSystemConfiguration(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap*                memoryMap,
  const struct XSDP*                     XSDP);

#endif