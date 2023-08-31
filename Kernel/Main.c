#include "Main.h"

static struct SystemConfiguration systemConfiguration;

void Main(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap*                memoryMap,
  const struct XSDP*                     XSDP) {
    code                        status;

    struct Message              message;

    SCREEN*                     screen = GetScreen();
    CONSOLE*                    systemConsole = GetSystemConsole();
    PCI_DEVICES*                PCIDevices = GetPCIDevices();
    MEMORY_MANAGER*             memoryManager = GetMemoryManager();
    HEAP*                       systemHeap = GetSystemHeap();
    TIMER_MANAGER*              timerManager = GetTimerManager();
    TASK_MANAGER*               taskManager = GetTaskManager();
    MESSAGE_QUEUE*              messageQueue = GetMessageQueue();

    ClearInterruptFlag();

    SetSystemConfiguration(frameBufferConfiguration, memoryMap, XSDP);

    if (InitialzeSegmentation() != SUCCESS) {
        return;
    }

    if (InitializeMemoryManager(&systemConfiguration.memoryMap) != SUCCESS) {
        return;
    }

    if (InitializeHeap(memoryManager) != SUCCESS) {
        return;
    }

    if (InitializeScreen(&systemConfiguration.frameBufferConfiguration) != SUCCESS) {
        return;
    }

    if (InitializeSystemConsole(screen) != SUCCESS) {
        return;
    }

    systemConsole->Print(systemConsole, "DummyOS Booting...\n");

    status = InitializeACPI(&systemConfiguration.XSDP);

    systemConsole->Print(systemConsole, "Initializing ACPI is %s.\n", Error(status));

    if (status != SUCCESS) {
        Pause();
    }

    status = InitializeTimerManager();

    systemConsole->Print(systemConsole, "Initializing timer manager is %s.\n", Error(status));

    if (status != SUCCESS) {
        Pause();
    }

    status = InitializeInterrupt();

    systemConsole->Print(systemConsole, "Initializing interrupt is %s.\n", Error(status));

    if (status != SUCCESS) {
        Pause();
    }

    status = InitializeTaskManager();

    systemConsole->Print(systemConsole, "Initializing task manager is %s.\n", Error(status));

    if (status != SUCCESS) {
        Pause();
    }

    status = InitializePCI();

    systemConsole->Print(systemConsole, "Initializing PCI device is %s.\n", Error(status));

    if (status != SUCCESS) {
        Pause();
    }

    timerManager->CreateTimer(timerManager, 10, 1);

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

void SetSystemConfiguration(
  const struct FrameBufferConfiguration* frameBufferConfiguration,
  const struct MemoryMap*                memoryMap,
  const struct XSDP*                     XSDP) {
    systemConfiguration.frameBufferConfiguration = *frameBufferConfiguration;
    systemConfiguration.memoryMap = *memoryMap;
    systemConfiguration.XSDP = *XSDP;

    return;
}