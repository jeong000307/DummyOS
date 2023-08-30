#include "Main.h"

static struct SystemConfiguration systemConfiguration;

void Main(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap* memoryMap) {
    struct SystemConfiguration* systemConfiguration = GetSystemConfiguration(frameBufferConfiguration, memoryMap);
    SCREEN*                     screen = GetScreen();
    CONSOLE*                    systemConsole = GetSystemConsole();
    PCI_DEVICES*                PCIDevices = GetPCIDevices();
    MEMORY_MANAGER*             memoryManager = GetMemoryManager();
    HEAP*                       systemHeap = GetSystemHeap();
    TIMER*                      systemTimer = GetTimer();
    TASK_MANAGER*               taskManager = GetTaskManager();
    MESSAGE_QUEUE*              messageQueue = GetMessageQueue();

    ClearInterruptFlag();

    if (InitialzeSegmentation() != SUCCESS) {
        return;
    }

    if (InitializeMemoryManager(&systemConfiguration->memoryMap) != SUCCESS) {
        return;
    }

    if (InitializeHeap(memoryManager) != SUCCESS) {
        return;
    }

    if (InitializeTimer() != SUCCESS) {
        return;
    }

    if (InitializeScreen(&systemConfiguration->frameBufferConfiguration) != SUCCESS) {
        return;
    }

    if (InitializeSystemConsole(screen) != SUCCESS) {
        return;
    }

    systemConsole->Print(systemConsole, "Booting...\n");

    systemConsole->Print(systemConsole, "Initializing interrupt is %s.\n", Error(InitializeInterrupt(messageQueue)));

    systemConsole->Print(systemConsole, "Initializing task manager is %s.\n", Error(InitializeTaskManager()));

    systemConsole->Print(systemConsole, "Initializing PCI device is %s.\n", Error(InitializePCI()));

    while(true) {
        ClearInterruptFlag();

        if (messageQueue->count == 0) {
            SetInterruptFlag();
            continue;
        }

        SetInterruptFlag();
    }

    Pause();
}

struct SystemConfiguration* GetSystemConfiguration(
  struct FrameBufferConfiguration* frameBufferConfiguration,
  struct MemoryMap*                memoryMap) {
    systemConfiguration.frameBufferConfiguration = *frameBufferConfiguration;
    systemConfiguration.memoryMap = *memoryMap;

    return &systemConfiguration;
}