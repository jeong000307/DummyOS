#include "Main.h"

static struct SystemConfiguration systemConfiguration;

void Main(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap* memoryMap) {
    struct Message message;

    struct SystemConfiguration* systemConfiguration = GetSystemConfiguration(frameBufferConfiguration, memoryMap);
    SCREEN*                     screen = GetScreen();
    CONSOLE*                    systemConsole = GetSystemConsole();
    PCI_DEVICES*                PCIDevices = GetPCIDevices();
    MEMORY_MANAGER*             memoryManager = GetMemoryManager();
    HEAP*                       systemHeap = GetSystemHeap();
    TIMER_MANAGER*              timerManager = GetTimerManager();
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

    if (InitializeScreen(&systemConfiguration->frameBufferConfiguration) != SUCCESS) {
        return;
    }

    if (InitializeSystemConsole(screen) != SUCCESS) {
        return;
    }

    systemConsole->Print(systemConsole, "Booting...\n");

    systemConsole->Print(systemConsole, "Initializing timer manager is %s.\n", Error(InitializeTimerManager()));

    systemConsole->Print(systemConsole, "Initializing interrupt is %s.\n", Error(InitializeInterrupt(messageQueue)));

    systemConsole->Print(systemConsole, "Initializing task manager is %s.\n", Error(InitializeTaskManager()));

    systemConsole->Print(systemConsole, "Initializing PCI device is %s.\n", Error(InitializePCI()));

    timerManager->CreateTimer(timerManager, 20, 2);
    timerManager->CreateTimer(timerManager, 60, -1);

    while(true) {
        ClearInterruptFlag();

        if (messageQueue->count == 0) {
            SetInterruptFlag();
            continue;
        }

        SetInterruptFlag();

        message = messageQueue->Pop(messageQueue);

        switch (message.type) {
            case TimerInterruptIndex:
                systemConsole->Print(systemConsole, "Timer: timeout = %u, value = %d\n", message.argument.timer.timeOut, message.argument.timer.value);

                if (message.argument.timer.value > 0) {
                    timerManager->CreateTimer(timerManager, message.argument.timer.timeOut + 10, message.argument.timer.value + 1);
                }
                break;
        }
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