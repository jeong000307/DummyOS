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

code InitializePCI(void);

PCI_DEVICES* GetPCIDevices(void);

static bool HasSingleFunctionPCIDevice(
    uint8 headerType);

static code ScanPCIBus(
    PCI_DEVICES* PCIDevice,
    uint8 bus);

static code ScanPCIDevice(
    PCI_DEVICES* PCIDevice,
    uint8 bus,
    uint8 device);

static code ScanPCIFunction(
    PCI_DEVICES* PCIDevice,
    uint8 bus,
    uint8 device,
    uint8 function);

static code AddPCIDevice(
    PCI_DEVICES* PCIDevice,
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

static uint32 MakePCIAddress(
    uint8 bus,
    uint8 device,
    uint8 function,
    uint8 registerAddress);

static void WritePCIAddress(
    uint32 address);

static void WritePCIData(
    uint32 value);

static uint32 ReadPCIData(void);

#endif