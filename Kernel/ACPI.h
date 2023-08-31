#ifndef __ACPI_H__
#define __ACPI_H__

#include "AssemblyFunction.h"
#include "Error.h"
#include "Memory.h"
#include "String.h"
#include "Type.h"

#pragma pack(push, 1)
struct XSDP {
    byte       signiture[8];
    uint8      checksum;
    byte       oemID[6];
    uint8      revision;
    uint32     RSDTAddress;
    uint32     length;
    uint64     XSDTAddress;
    uint8      extendedChecksum;
    byte       reserved[3];
};
#pragma pack(pop)

typedef bool (*CHECK_SDT)(
    const struct ACPI_SDT_HEADER* this,
    const byte* expectedSignature);

#pragma pack(push, 1)
struct ACPI_SDT_HEADER {
    byte   signiture[4];
    uint32 length;
    uint8  revision;
    uint8  checksum;
    byte   oemID[6];
    byte   oemTableID[8];
    uint32 oemRevision;
    uint32 creatorID;
    uint32 creatorRevision;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct XSDT {
    struct ACPI_SDT_HEADER header;

    addr OtherSDTPointer;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GenericAddressStructure {
    uint8 addressSpace;
    uint8 bitWidth;
    uint8 bitOffset;
    uint8 accessSize;
    uint64 address;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FADT {
    struct ACPI_SDT_HEADER header;
    uint32 firmwareCtrl;
    uint32 DSDT;

    uint8  reserved;

    uint8  preferredPowerManagementProfile;
    uint16 SCIInterrupt;
    uint32 SMICommandPort;
    uint8  ACPIEnable;
    uint8  ACPIDisable;
    uint8  S4BIOSREQ;
    uint8  PStateControl;
    uint32 PM1aEventBlock;
    uint32 PM1bEventBlock;
    uint32 PM1aControlBlock;
    uint32 PM1bControlBlock;
    uint32 PM2ControlBlock;
    uint32 PMTimerBlock;
    uint32 GPE0Block;
    uint32 GPE1Block;
    uint8  PM1EventLength;
    uint8  PM1ControlLength;
    uint8  PM2ControlLength;
    uint8  PMTimerLength;
    uint8  GPE0Length;
    uint8  GPE1Length;
    uint8  GPE1Base;
    uint8  CStateControl;
    uint16 worstC2Latency;
    uint16 worstC3Latency;
    uint16 flushSize;
    uint16 flushStride;
    uint8  dutyOffset;
    uint8  dutyWidth;
    uint8  dayAlarm;
    uint8  monthAlarm;
    uint8  century;

    uint16 bootArchitectureFlags;

    uint8  reserved2;
    uint32 flags;

    struct GenericAddressStructure resetRegister;

    uint8  ResetValue;
    uint8  Reserved3[3];

    uint64                XFirmwareControl;
    uint64                XDSDT;

    struct GenericAddressStructure XPM1aEventBlock;
    struct GenericAddressStructure XPM1bEventBlock;
    struct GenericAddressStructure XPM1aControlBlock;
    struct GenericAddressStructure XPM1bControlBlock;
    struct GenericAddressStructure XPM2ControlBlock;
    struct GenericAddressStructure XPMTimerBlock;
    struct GenericAddressStructure XGPE0Block;
    struct GenericAddressStructure XGPE1Block;
    struct GenericAddressStructure SleepControlRegister;
    struct GenericAddressStructure SleepStatusRegister;

    uint64 hypervisorVendorIdentity;
};
#pragma pack(pop)

struct XSDP* GetACPITable(void);

code InitializeACPI(
  struct XSDP* XSDP);

void* FindFADT(struct XSDT* XSDT);

bool CheckXSDP(
  const struct XSDP* XSDP);

bool CheckSDT(
  const struct ACPI_SDT_HEADER* this,
  const byte* expectedSignature);

#endif