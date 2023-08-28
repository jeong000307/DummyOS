#include "Main.h"

void Main(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfiguration,
  IN const struct MEMORY_MAP* memoryMap) {
    code           status;
    size           index;
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

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing screen and system console is success.\n");

    status = InitialzeSegmentation();

    if (status != SUCCESS) {
        return;
    }

    status = InitializeMemoryManager(memoryMap);

    if (status != SUCCESS) {
        return;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing memory manager is ");

    if (status != SUCCESS) {
        systemConsole->SystemPrint(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "success.\n");

    memoryManager = GetMemoryManager();

    status = InitializeHeap(memoryManager);

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing system heap is ");

    if (status != SUCCESS) {
        systemConsole->SystemPrint(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "success.\n");

    systemHeap = GetSystemHeap();

    status = InitializeInterrupt();

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing Interrupt is ");

    if (status != SUCCESS) {
        systemConsole->SystemPrint(systemConsole, "%s", "failed.\n");
        return ;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "success.\n");

    status = InitializePCI();

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing PCI device is ");

    if (status != SUCCESS) {
        systemConsole->SystemPrint(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "success.\n");

    PCIDevices = GetPCIDevices();

    for (index = 0; index < PCIDevices->count; ++index) {
        systemConsole->SystemPrint(systemConsole, "%d.%d.%d: vend %x, class %x, head %x\n", 
          PCIDevices->devices[index]->bus, 
          PCIDevices->devices[index]->device, 
          PCIDevices->devices[index]->function, 
          ReadPCIVendorID(
            PCIDevices->devices[index]->bus,
            PCIDevices->devices[index]->device,
            PCIDevices->devices[index]->function),
          ReadPCIClassCode(
            PCIDevices->devices[index]->bus,
            PCIDevices->devices[index]->device,
            PCIDevices->devices[index]->function), 
          PCIDevices->devices[index]->headerType);
    }

    systemConsole->SystemPrint(systemConsole, "%d", systemTimer->CountTime(systemTimer));

    systemTimer->StopTimer(systemTimer);

    Pause();
}