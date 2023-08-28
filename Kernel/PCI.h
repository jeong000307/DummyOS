#ifndef __PCI_H__
#define __PCI_H__

#include "Type.h"
#include "Console.h"
#include "Error.h"

typedef struct __PCI_DEVICES PCI_DEVICES;

struct PCIDevice {
    uint8 bus, device, function, headerType;
};

struct __PCI_DEVICES {
    struct PCIDevice* devices[32];
    size              count;
};

union CapabilityHeader {
    uint32 data;
    struct {
        unsigned capabilityID: 8;
        unsigned nextPointer: 8;
        unsigned capability: 16;
    } bits;
};

struct MSICapability {
    union {
        uint32 data;
        struct {
            unsigned capabilityID: 8;
            unsigned nextPointer: 8;
            unsigned MSIEnable: 1;
            unsigned multiMessageCapable: 3;
            unsigned multiMessageEnable: 3;
            unsigned Address64Capable: 1;
            unsigned perVectorMaskCapable: 1;
            unsigned : 7;
        } bits;
    } header;

    uint32 messageAddress;
    uint32 messageUpperAddress;
    uint32 messageData;
    uint32 maskBits;
    uint32 pendingBits;
};

enum MSITriggerMode {
    Edge = 0,
    Level = 1
};

enum MSIDeliveryMode {
    Fixed = 0b000,
    LowestPriority = 0b001,
    SMI = 0b010,
    NMI = 0b100,
    INIT = 0b101,
    ExtINT = 0b111
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

static uint64 ReadPCIBAR(
  struct PCIDevice* device, 
  uint64 BARIndex);

static uint8 GetPCIBARAddress(
  uint64 BARIndex);

static uint32 ReadConfigurationRegister(
  const struct PCIDevice* device, 
  uint8 registerAddress);

static void WriteConfigurationRegister(
  const struct PCIDevice* device, 
  uint8 registerAddress, 
  uint32 value);

static union CapabilityHeader ReadCapabilityHeader(
  const struct PCIDevice* device, 
  uint8 address);

void ConfigureMSIFixedDestination(
  const struct PCIDevice* device,
  uint8 APICID,
  enum MSITriggerMode triggerMode,
  enum MSIDeliveryMode deliveryMode,
  uint8 vector,
  uint64 numberOfVectorExponent);

void ConfigureMSI(
  const struct PCIDevice* device,
  uint32 messageAddress,
  uint32 messageData,
  uint64 numberOfVectorExponent);

#endif