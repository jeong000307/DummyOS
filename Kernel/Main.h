#ifndef __MAIN_H__
#define __MAIN_H__

#include "AssemblyFunction.h"
#include "Console.h"
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
};

struct SystemConfiguration* GetSystemConfiguration(
  struct FrameBufferConfiguration* frameBufferConfiguration,
  struct MemoryMap*                memoryMap);

#endif