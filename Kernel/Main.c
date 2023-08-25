#include "Main.h"

SYSTEM_CONSOLE* systemConsole;
PCI_DEVICES*    PCIDevices;

void Main(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfig) {
    code            status;
    size            index;

    SCREEN*         screen = &(SCREEN){ 
      .pixelsPerScanLine = 0, 
      .horizontalResolution = 0, 
      .verticalResolution = 0, 
      .frameBuffer = NULL, 
      .WritePixel = NULL, 
      .GetPixelAddress = NULL 
    };

    status = CreateScreen(
      screen,
      frameBufferConfig);

    if (status != SUCCESS) {
        return ;
    }

    status = CreateSystemConsole(
      systemConsole,
      screen
    );

    if (status != SUCCESS) {
        return ;
    }

    systemConsole->SystemPrint(systemConsole, "%s", "Hello from DummyOS!\n");

    ScanAllPCIBus();

    systemConsole->SystemPrint(systemConsole, "ScanAllBus: %d\n", PCIDevices->count);

    for (index = 0; index < PCIDevices->count; ++index) {
        struct PCIDevice* device = PCIDevices->devices[index];

        uint16 vendorID = ReadPCIVendorID(device->bus, device->device, device->function);
        uint32 classCode = ReadPCIClassCode(device->bus, device->device, device->function);

        systemConsole->SystemPrint(systemConsole, "%d.%d.%d: vend %x, class %x, head %x\n", device->bus, device->device, device->function, vendorID, classCode, device->headerType);
    }
    
    Halt();
}