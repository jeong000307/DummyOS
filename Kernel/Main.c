#include "Main.h"

void Main(
  const struct FrameBufferConfig* frameBufferConfiguration,
  const struct MemoryMap* memoryMap) {
    SCREEN*         screen = GetScreen();
    CONSOLE*        systemConsole = GetSystemConsole();
    PCI_DEVICES*    PCIDevices = GetPCIDevices();
    MEMORY_MANAGER* memoryManager = GetMemoryManager();
    HEAP*           systemHeap = GetSystemHeap();
    TIMER*          systemTimer = GetTimer();
    TASK_MANAGER*   taskManager = GetTaskManager();
    MESSAGE_QUEUE*  messageQueue = GetMessageQueue();
    struct Message  message;

    if (InitializeTimer() != SUCCESS) {
        return;
    }

    if (InitializeScreen(frameBufferConfiguration) != SUCCESS) {
        return;
    }

    if (InitializeSystemConsole(screen) != SUCCESS) {
        return;
    }

    systemConsole->Print(systemConsole, "Initializing screen and system console is success.\n");

    systemConsole->Print(systemConsole, "Initializing segmentation is %s.\n", Error(InitialzeSegmentation()));

    systemConsole->Print(systemConsole, "Initializing memory manager is %s.\n", Error(InitializeMemoryManager(memoryMap)));

    systemConsole->Print(systemConsole, "Initializing system heap is %s.\n", Error(InitializeHeap(memoryManager)));

    systemConsole->Print(systemConsole, "Initializing interrupt is %s.\n", Error(InitializeInterrupt(messageQueue)));

    systemConsole->Print(systemConsole, "Initializing task manager is %s.\n", Error(InitializeTaskManager()));

    systemConsole->Print(systemConsole, "Initializing PCI device is %s.\n", Error(InitializePCI()));

    while(true) {
        ClearInterruptFlag();

        if (messageQueue->count == 0) {
            SetInterruptFlag();
            continue;
        }

        message = messageQueue->Pop(messageQueue);
        SetInterruptFlag();

        switch (message.type) {
            case TimerInterruptIndex:
                systemConsole->Print(systemConsole, "INTERRUPT\n");
                break;
        }
    }

    Pause();
}