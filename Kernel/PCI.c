#include "PCI.h"

extern SYSTEM_CONSOLE* systemConsole;
extern PCI_DEVICES*    PCIDevices;

static const uint8 maxSizeOfPCIDevice = 32;
static const uint8 maxSizeOfPCIFunction = 8;

static const uint16 PCIConfigAddress = 0x0cf8;
static const uint16 PCIConfigData = 0x0cfc;

code ScanAllPCIBus() {
    code status;
    uint8 headerType = ReadPCIHeaderType(0, 0, 0);

    PCIDevices->count = 0;

    if (HasSingleFunctionPCIDevice(headerType)) {
        return ScanPCIBus(0);
    }

    for (uint8 function = 1; function < maxSizeOfPCIFunction; ++function) {
        if (ReadPCIVendorID(0, 0, function) == 0xffffu) {
            continue;
        }

        status = ScanPCIBus(function);

        if (status == PCI_ERROR) {
            return PCI_ERROR;
        }
    }

    return SUCCESS;
}

bool HasSingleFunctionPCIDevice(
  uint8 headerType) {
    return (headerType & 0x80u) == 0;
}

code ScanPCIBus(
  uint8 bus) {
    code status = SUCCESS;

    for (uint8 device = 0; device < maxSizeOfPCIDevice; ++device) {
        if (ReadPCIVendorID(bus, device, 0) == 0xffffu) {
            continue;
        }

        status = ScanPCIDevice(bus, device);

        if (status == PCI_ERROR) {
            return status;
        }
    }

    return status;
}

code ScanPCIDevice(
  uint8 bus, 
  uint8 device) {
    code status;

    status = ScanPCIFunction(bus, device, 0);

    if (status == PCI_ERROR) {
        return PCI_ERROR;
    }

    if (HasSingleFunctionPCIDevice(ReadPCIHeaderType(bus, device, 0))) {
        return SUCCESS;
    }

    for (uint8 function = 1; function < maxSizeOfPCIFunction; ++function) {
        if (ReadPCIVendorID(bus, device, function) == 0xffffu) {
            continue;
        }

        status = ScanPCIFunction(bus, device, function);

        if (status == PCI_ERROR) {
            return PCI_ERROR;
        }
    }

    return SUCCESS;
}

code ScanPCIFunction(
    uint8 bus,
    uint8 device,
    uint8 function) {
    code status;
    uint8 headerType = ReadPCIHeaderType(bus, device, function);
    uint8 baseClassCode, subClassCode;
    uint8 secondaryBusNumber;
    uint32 classCode;
    uint32 busNumber;

    status = AddPCIDevice(bus, device, function, headerType);

    if (status == PCI_ERROR) {
        return PCI_ERROR;
    }

    classCode = ReadPCIClassCode(bus, device, function);
    baseClassCode = (classCode >> 24) & 0xffu;
    subClassCode = (classCode >> 16) & 0xffu;

    if (baseClassCode == 0x06u
        and subClassCode == 0x04u) {
        busNumber = ReadPCIBusNumber(bus, device, function);
        secondaryBusNumber = (busNumber >> 8) & 0xffu;

        return ScanPCIBus(secondaryBusNumber);
    }

    return SUCCESS;
}

code AddPCIDevice(
    uint8 bus,
    uint8 device,
    uint8 function,
    uint8 headerType) {
    if (PCIDevices->count == maxSizeOfPCIDevice) {
        return PCI_ERROR;
    }

    PCIDevices->devices[PCIDevices->count]->bus = bus;
    PCIDevices->devices[PCIDevices->count]->device = device;
    PCIDevices->devices[PCIDevices->count]->function = function;
    PCIDevices->devices[PCIDevices->count]->headerType = headerType;

    ++PCIDevices->count;

    return SUCCESS;
}

uint16 ReadPCIVendorID(
    uint8 bus,
    uint8 device,
    uint8 function) {
    WritePCIAddress(MakePCIAddress(bus, device, function, 0x00));

    return ReadPCIData() & 0xffffu;
}

uint16 ReadPCIDeviceID(
  uint8 bus,
  uint8 device,
  uint8 function) {
    WritePCIAddress(MakePCIAddress(bus, device, function, 0x00));

    return ReadPCIData() >> 16;
}

uint32 ReadPCIClassCode(
  uint8 bus, 
  uint8 device, 
  uint8 function) {
    WritePCIAddress(MakePCIAddress(bus, device, function, 0x08));

    return ReadPCIData();
}

uint8 ReadPCIHeaderType(
    uint8 bus,
    uint8 device,
    uint8 function) {
    WritePCIAddress(MakePCIAddress(bus, device, function, 0x0c));

    return (ReadPCIData() >> 16) & 0xffu;
}

uint32 ReadPCIBusNumber(
  uint8 bus, 
  uint8 device, 
  uint8 function) {
    WritePCIAddress(MakePCIAddress(bus, device, function, 0x18));

    return ReadPCIData();
}

uint32 MakePCIAddress(
    uint8 bus,
    uint8 device,
    uint8 function,
    uint8 registerAddress) {
    return (1 << 31) 
      | (bus << 16)
      | (device << 11)
      | (function << 8)
      | (registerAddress & 0xfcu);
}

void WritePCIAddress(
    uint32 address) {
    IOOut32(PCIConfigAddress, address);
}

void WritePCIData(
    uint32 value) {
    IOOut32(PCIConfigData, value);
}

uint32 ReadPCIData() {
    return IOIn32(PCIConfigData);
}