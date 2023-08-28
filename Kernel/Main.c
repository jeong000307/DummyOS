#include "Main.h"

void Main(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfiguration,
  IN const struct MEMORY_MAP* memoryMap) {
    code           status;

    SCREEN*         screen;
    SYSTEM_CONSOLE* systemConsole;
    PCI_DEVICES*    PCIDevices;
    MEMORY_MANAGER* memoryManager;
    HEAP*           systemHeap;
    TIMER*          systemTimer;

    status = InitializeTimer();

    if (status != SUCCESS) {
        return ;
    }

    systemTimer = GetTimer();

    systemTimer->StartTimer(systemTimer);

    status = InitializeScreen(frameBufferConfiguration);

    if (status != SUCCESS) {
        return;
    }

    screen = GetScreen();

    status = InitializeSystemConsole(screen);

    if (status != SUCCESS) {
        return;
    }

    systemConsole = GetSystemConsole();

    systemConsole->Print(systemConsole, "%s", "Initializing screen and system console is success.\n");

    status = InitialzeSegmentation();

    systemConsole->Print(systemConsole, "%s", "Initializing segmentation is ");

    if (status != SUCCESS) {
        systemConsole->Print(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->Print(systemConsole, "%s", "success.\n");

    status = InitializeMemoryManager(memoryMap);

    systemConsole->Print(systemConsole, "%s", "Initializing memory manager is ");

    if (status != SUCCESS) {
        systemConsole->Print(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->Print(systemConsole, "%s", "success.\n");

    memoryManager = GetMemoryManager();

    status = InitializeHeap(memoryManager);

    systemConsole->Print(systemConsole, "%s", "Initializing system heap is ");

    if (status != SUCCESS) {
        systemConsole->Print(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->Print(systemConsole, "%s", "success.\n");

    systemHeap = GetSystemHeap();

    status = InitializeInterrupt();

    systemConsole->Print(systemConsole, "%s", "Initializing Interrupt is ");

    if (status != SUCCESS) {
        systemConsole->Print(systemConsole, "%s", "failed.\n");
        return ;
    }

    systemConsole->Print(systemConsole, "%s", "success.\n");

    status = InitializePCI();

    systemConsole->Print(systemConsole, "%s", "Initializing PCI device is ");

    if (status != SUCCESS) {
        systemConsole->Print(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->Print(systemConsole, "%s", "success.\n");

    PCIDevices = GetPCIDevices();

    systemConsole->Print(systemConsole, "%d", systemTimer->CountTime(systemTimer));

    systemTimer->StopTimer(systemTimer);

    Pause();
}