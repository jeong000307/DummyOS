#ifndef __PCI_H__
#define __PCI_H__

#include "Type.h"
#include "Console.h"
#include "Error.h"

#define PCI_CONFIG_ADDRESS 0x0cf8
#define PCI_CONFIG_DATA    0x0cfc
#define PCI_DEVICE_MAX     32
#define PCI_FUNCTION_MAX   8

typedef struct __PCI_DEVICES PCI_DEVICES;

typedef code (*SCAN_PCI_BUS)(
  PCI_DEVICES* this,
  uint8        bus);

typedef code (*SCAN_PCI_DEVICE)(
  PCI_DEVICES* this,
  uint8        bus,
  uint8        device);

typedef code (*SCAN_PCI_FUNCTION)(
  PCI_DEVICES* this,
  uint8        bus,
  uint8        device,
  uint8        function);

typedef code (*ADD_PCI_DEVICE)(
  PCI_DEVICES* this,
  uint8        bus,
  uint8        device,
  uint8        function,
  uint8        headerType);

struct PCIDevice {
    uint8 bus, device, function, headerType;
};

struct __PCI_DEVICES {
    struct PCIDevice  devices[PCI_DEVICE_MAX];
    size              count;

    SCAN_PCI_BUS      ScanPCIBus;
    SCAN_PCI_DEVICE   ScanPCIDevice;
    SCAN_PCI_FUNCTION ScanPCIFunction;
    ADD_PCI_DEVICE    AddPCIDevice;
};

#pragma pack(push, 1)
union CapabilityHeader {
    uint32 data;
    struct {
        unsigned capabilityID: 8;
        unsigned nextPointer: 8;
        unsigned capability: 16;
    } bits;
};
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

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

static code __ScanPCIBus(
    PCI_DEVICES* this,
    uint8 bus);

static code __ScanPCIDevice(
    PCI_DEVICES* this,
    uint8 bus,
    uint8 device);

static code __ScanPCIFunction(
    PCI_DEVICES* this,
    uint8 bus,
    uint8 device,
    uint8 function);

static code __AddPCIDevice(
    PCI_DEVICES* this,
    uint8 bus,
    uint8 device,
    uint8 function,
    uint8 headerType);

#endif