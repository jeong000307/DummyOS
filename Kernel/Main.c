#include "Main.h"

void MainAP(void) {
    MEMORY_MANAGER* memoryManager = GetMemoryManager();

    InitializeSegmentRegister(1 << 3, 2 << 3, 0);
    SetCR3((addr)&memoryManager->PML4Table[0]);

    Pause();
}

void MainBSP(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap*                memoryMap,
  const struct RSDP*                     RSDP) {
    code                        status;

    struct Message              message;
    
    struct SystemConfiguration* systemConfiguration = GetSystemConfiguration();
    SCREEN*                     screen = GetScreen();
    CONSOLE*                    systemConsole = GetSystemConsole();
    PCI_DEVICES*                PCIDevices = GetPCIDevices();
    MEMORY_MANAGER*             memoryManager = GetMemoryManager();
    HEAP*                       systemHeap = GetSystemHeap();
    TIMER_MANAGER*              timerManager = GetTimerManager();
    TASK_MANAGER*               taskManager = GetTaskManager();
    MESSAGE_QUEUE*              messageQueue = GetMessageQueue();

    SetInterruptFlag(false);

    SetSystemConfiguration(frameBufferConfiguration, memoryMap, RSDP);

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

    systemConsole->Print(systemConsole, "DummyOS Booting...\n");

    ParseACPITable(&systemConfiguration->RSDP);

    InitializeTimerManager();

    systemConsole->Print(systemConsole, "Timer manager is initialized.\n");

    InitializeInterrupt();

    systemConsole->Print(systemConsole, "Interrupt manager is initialized.\n");

    status = InitializeTaskManager();

    systemConsole->Print(systemConsole, "Task manager is initialized.\n");

    status = InitializePCI();

    systemConsole->Print(systemConsole, "PCI devices are initialized.\n");

    timerManager->CreateTimer(timerManager, 10, taskTimerValue);

    while(true) {
        SetInterruptFlag(false);

        if (messageQueue->count == 0) {
            SetInterruptFlag(true);
            continue;
        }

        SetInterruptFlag(true);

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