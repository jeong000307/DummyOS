#include "Main.h"

void Main(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfig,
  IN const struct MEMORY_MAP* memoryMap) {
    code           status;
    size           index;
    SCREEN*         screen;
    SYSTEM_CONSOLE* systemConsole;
    PCI_DEVICES*    PCIDevices;
    MEMORY_MANAGER* memoryManager;
    HEAP*           systemHeap;

    status = InitializeScreen(frameBufferConfig);

    if (status != SUCCESS) {
        return;
    }

    screen = GetScreen();

    status = InitializeSystemConsole(screen);

    if (status != SUCCESS) {
        return;
    }

    systemConsole = GetSystemConsole();

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing screen and system console is complete.\n");

    status = SetupSegments();

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

    systemConsole->SystemPrint(systemConsole, "%s", "complete.\n");

    memoryManager = GetMemoryManager();

    status = InitializeHeap(memoryManager);

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing system heap is ");

    if (status != SUCCESS) {
        systemConsole->SystemPrint(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "complete.\n");

    systemHeap = GetSystemHeap();

    status = InitializePCI();

    systemConsole->SystemPrint(systemConsole, "%s", "Initializing PCI device is ");

    if (status != SUCCESS) {
        systemConsole->SystemPrint(systemConsole, "%s", "failed.\n");
        return;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "complete.\n");

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

    Pause();
}