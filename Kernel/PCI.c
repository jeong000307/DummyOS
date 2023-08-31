#include "PCI.h"
#include "Timer.h"

static PCI_DEVICES PCIDevices;

PCI_DEVICES* GetPCIDevices(void) {
    return &PCIDevices;
}

code InitializePCI(void) {
    code  status;
    uint8 headerType = ReadPCIHeaderType(0, 0, 0);

    PCIDevices.ScanPCIBus = __ScanPCIBus;
    PCIDevices.ScanPCIDevice = __ScanPCIDevice;
    PCIDevices.ScanPCIFunction = __ScanPCIFunction;
    PCIDevices.AddPCIDevice = __AddPCIDevice;

    PCIDevices.count = 0;

    if (HasSingleFunctionPCIDevice(headerType)) {
        return PCIDevices.ScanPCIBus(&PCIDevices, 0);
    }

    for (uint8 function = 1; function < PCI_FUNCTION_MAX; ++function) {
        if (ReadPCIVendorID(0, 0, function) == 0xffffu) {
            continue;
        }

        status = PCIDevices.ScanPCIBus(&PCIDevices, function);

        if (status == PCI_ERROR) {
            return PCI_ERROR;
        }
    }

    return SUCCESS;
}

static bool HasSingleFunctionPCIDevice(
  uint8 headerType) {
    return (headerType & 0x80u) == 0;
}

static code __ScanPCIBus(
  PCI_DEVICES* this,
  uint8        bus) {
    code status;

    for (uint8 device = 0; device < PCI_DEVICE_MAX; ++device) {
        if (ReadPCIVendorID(bus, device, 0) == 0xffffu) {
            continue;
        }

        status = this->ScanPCIDevice(this, bus, device);

        if (status == PCI_ERROR) {
            return status;
        }
    }

    return SUCCESS;
}

static code __ScanPCIDevice(
  PCI_DEVICES* this,
  uint8        bus, 
  uint8        device) {
    code status;

    status = this->ScanPCIFunction(this, bus, device, 0);

    if (status == PCI_ERROR) {
        return PCI_ERROR;
    }

    if (HasSingleFunctionPCIDevice(ReadPCIHeaderType(bus, device, 0))) {
        return SUCCESS;
    }

    for (uint8 function = 1; function < PCI_FUNCTION_MAX; ++function) {
        if (ReadPCIVendorID(bus, device, function) == 0xffffu) {
            continue;
        }

        status = this->ScanPCIFunction(this, bus, device, function);

        if (status == PCI_ERROR) {
            return PCI_ERROR;
        }
    }

    return SUCCESS;
}

static code __ScanPCIFunction(
  PCI_DEVICES* this,
  uint8        bus,
  uint8        device,
  uint8        function) {
    code   status;
    uint8  headerType = ReadPCIHeaderType(bus, device, function);
    uint8  baseClassCode;
    uint8  subClassCode;
    uint8  secondaryBusNumber;
    uint32 classCode;
    uint32 busNumber;

    status = this->AddPCIDevice(this, bus, device, function, headerType);

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

        return this->ScanPCIBus(this, secondaryBusNumber);
    }

    return SUCCESS;
}

static code __AddPCIDevice(
  PCI_DEVICES* this,
  uint8        bus,
  uint8        device,
  uint8        function,
  uint8        headerType) {
    if (this->count == PCI_DEVICE_MAX) {
        return PCI_ERROR;
    }

    this->devices[this->count].bus = bus;
    this->devices[this->count].device = device;
    this->devices[this->count].function = function;
    this->devices[this->count].headerType = headerType;

    ++this->count;

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

static uint32 MakePCIAddress(
  uint8 bus,
  uint8 device,
  uint8 function,
  uint8 registerAddress) {
    return (1 << 31) | (bus << 16) | (device << 11) | (function << 8) | (registerAddress & 0xfcu);
}

static void WritePCIAddress(
  uint32 address) {
    IOOut32(PCI_CONFIG_ADDRESS, address);
}

static void WritePCIData(
  uint32 value) {
    IOOut32(PCI_CONFIG_DATA, value);
}

static uint32 ReadPCIData(void) {
    return IOIn32(PCI_CONFIG_DATA);
}