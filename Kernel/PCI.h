#ifndef __PCI_H__
#define __PCI_H__

#include "Type.h"
#include "Console.h"
#include "ErrorCode.h"

typedef struct __PCI_DEVICES PCI_DEVICES;

struct PCIDevice {
    uint8 bus, device, function, headerType;
};

struct __PCI_DEVICES {
    struct PCIDevice* devices[32];
    size              count;
};

code ScanAllPCIBus();

bool HasSingleFunctionPCIDevice(
    uint8 headerType);

code ScanPCIBus(
    uint8 bus);

code ScanPCIDevice(
    uint8 bus,
    uint8 device);

code ScanPCIFunction(
    uint8 bus,
    uint8 device,
    uint8 function);

code AddPCIDevice(
    uint8 bus,
    uint8 device,
    uint8 function,
    uint8 headerType);

uint16 ReadPCIVendorID(
    uint8 bus,
    uint8 device,
    uint8 function);

uint16 ReadPCIDeviceID(
    uint8 bus,
    uint8 device,
    uint8 function);

uint32 ReadPCIClassCode(
    uint8 bus,
    uint8 device,
    uint8 function);

uint8 ReadPCIHeaderType(
    uint8 bus,
    uint8 device,
    uint8 function);

uint32 ReadPCIBusNumber(
    uint8 bus,
    uint8 device,
    uint8 function);

uint32 MakePCIAddress(
    uint8 bus,
    uint8 device,
    uint8 function,
    uint8 registerAddress);

void WritePCIAddress(
    uint32 address);

void WritePCIData(
    uint32 value);

uint32 ReadPCIData();

#endif