#include "PCI.h"

static PCI_DEVICES* PCIDevices;

code InitializePCI(void) {
    code status;
    uint8 headerType = ReadPCIHeaderType(0, 0, 0);

    PCIDevices->count = 0;

    if (HasSingleFunctionPCIDevice(headerType)) {
        return ScanPCIBus(PCIDevices, 0);
    }

    for (uint8 function = 1; function < PCI_FUNCTION_MAX; ++function) {
        if (ReadPCIVendorID(0, 0, function) == 0xffffu) {
            continue;
        }

        status = ScanPCIBus(PCIDevices, function);

        if (status == PCI_ERROR) {
            return PCI_ERROR;
        }
    }

    return SUCCESS;
}

PCI_DEVICES* GetPCIDevices(void) {
    return PCIDevices;
}

static bool HasSingleFunctionPCIDevice(
  uint8 headerType) {
    return (headerType & 0x80u) == 0;
}

static code ScanPCIBus(
  PCI_DEVICES* PCIDevices,
  uint8 bus) {
    code status = SUCCESS;

    for (uint8 device = 0; device < PCI_DEVICE_MAX; ++device) {
        if (ReadPCIVendorID(bus, device, 0) == 0xffffu) {
            continue;
        }

        status = ScanPCIDevice(PCIDevices, bus, device);

        if (status == PCI_ERROR) {
            return status;
        }
    }

    return status;
}

static code ScanPCIDevice(
  PCI_DEVICES* PCIDevices,
  uint8 bus, 
  uint8 device) {
    code status;

    status = ScanPCIFunction(PCIDevices, bus, device, 0);

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

        status = ScanPCIFunction(PCIDevices, bus, device, function);

        if (status == PCI_ERROR) {
            return PCI_ERROR;
        }
    }

    return SUCCESS;
}

static code ScanPCIFunction(
  PCI_DEVICES* PCIDevices,
  uint8 bus,
  uint8 device,
  uint8 function) {
    code status;
    uint8 headerType = ReadPCIHeaderType(bus, device, function);
    uint8 baseClassCode, subClassCode;
    uint8 secondaryBusNumber;
    uint32 classCode;
    uint32 busNumber;

    status = AddPCIDevice(PCIDevices, bus, device, function, headerType);

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

        return ScanPCIBus(PCIDevices, secondaryBusNumber);
    }

    return SUCCESS;
}

static code AddPCIDevice(
  PCI_DEVICES* PCIDevices,
  uint8 bus,
  uint8 device,
  uint8 function,
  uint8 headerType) {
    if (PCIDevices->count == PCI_DEVICE_MAX) {
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

static uint32 MakePCIAddress(
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

static uint64 ReadPCIBAR(struct PCIDevice* device, uint64 BARIndex) {
    uint64 BARAddress;
    uint32 BAR;
    uint32 BARUpper;

    if (BARIndex >= 6) {
        return 0;
    }

    BARAddress = GetPCIBARAddress(BARIndex);
    BAR = ReadConfigurationRegister(device, BARAddress);

    if ((BAR & 4u) == 0) {
        return BAR;
    }

    if (BARIndex >= 5) {
        return 0;
    }

    BARUpper = ReadConfigurationRegister(device, BARAddress + 4);
    
    return BAR | ((uint64)BARUpper << 32);
}

static uint8 GetPCIBARAddress(uint64 BARIndex) {
    return 0x10 + 4 * BARIndex;
}

static uint32 ReadConfigurationRegister(const struct PCIDevice* device, uint8 registerAddress) {
    WritePCIAddress(MakePCIAddress(device->bus, device->device, device->function, registerAddress));

    return ReadPCIData();
}

static void WriteConfigurationRegister(const struct PCIDevice* device, uint8 registerAddress, uint32 value) {
    WritePCIAddress(MakePCIAddress(device->bus, device->device, device->function, registerAddress));
    
    WritePCIData(value);
}

static union CapabilityHeader ReadCapabilityHeader(const struct PCIDevice* device, uint8 address) {
    union CapabilityHeader header;

    header.data = ReadConfigurationRegister(device, address);

    return header;
}

void ConfigureMSIFixedDestination(
  const struct PCIDevice* device, 
  uint8 APICID, 
  enum MSITriggerMode triggerMode, 
  enum MSIDeliveryMode deliveryMode, 
  uint8 vector, 
  uint64 numberOfVectorExponent) {
    uint32 messageAddress = 0xfee00000u | (APICID << 12);
    uint32 messageData = (uint32)deliveryMode << 8 | vector;

    if (triggerMode == Level) {
        messageData |= 0xc000;
    }

    ConfigureMSI(device, messageAddress, messageData, numberOfVectorExponent);
}

void ConfigureMSI(
  const struct PCIDevice* device,
  uint32 messageAddress,
  uint32 messageData, 
  uint64 numberOfVectorExponent) {
    uint8 capabilityAddress = ReadConfigurationRegister(device, 0x34) & 0xffu;
    uint8 MSICapabilityAddress = 0;
    uint8 MSIXCapabilityAddress = 0;

    while (capabilityAddress != 0) {

    }
}