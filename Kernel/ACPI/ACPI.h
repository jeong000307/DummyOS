#pragma once

#include "../AssemblyFunction.h"
#include "../Error.h"
#include "../Memory.h"
#include "../String.h"
#include "../SystemConfiguration.h"
#include "../Type.h"

#pragma pack(push, 1)
struct GAS {
    uint8  addressSpaceID;
    uint8  registerBitWidth;
    uint8  registerBitOffset;
    uint8  accessSize;
    uint64 address;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DescriptionHeader {
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
    struct DescriptionHeader header;

    addr                     entry[1];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FADT {
    struct DescriptionHeader header;
    uint32                   firmwareCtrl;
    uint32                   DSDT;
    uint8                    reserved;
    uint8                    preferredPMProfile;
    uint16                   SCIInterrupt;
    uint32                   SMICommandPort;
    uint8                    ACPIEnable;
    uint8                    ACPIDisable;
    uint8                    S4BIOSRequest;
    uint8                    PStateControl;
    uint32                   PM1aEventBlock;
    uint32                   PM1bEventBlock;
    uint32                   PM1aControlBlock;
    uint32                   PM1bControlBlock;
    uint32                   PM2ControlBlock;
    uint32                   PMTimerBlock;
    uint32                   GPE0Block;
    uint32                   GPE1Block;
    uint8                    PM1EventLength;
    uint8                    PM1ControlLength;
    uint8                    PM2ControlLength;
    uint8                    PMTimerLength;
    uint8                    GPE0Length;
    uint8                    GPE1Length;
    uint8                    GPE1Base;
    uint8                    CStateControl;
    uint16                   worstC2Latency;
    uint16                   worstC3Latency;
    uint16                   flushSize;
    uint16                   flushStride;
    uint8                    dutyOffset;
    uint8                    dutyWidth;
    uint8                    dayAlarm;
    uint8                    monthAlarm;
    uint8                    century;
    uint16                   bootArchitectureFlags;
    uint8                    reserved2;
    uint32                   flags;
    struct GAS               resetRegister;
    uint8                    resetValue;
    uint8                    reserved3[3];
    uint64                   XFirmwareControl;
    uint64                   XDSDT;
    struct GAS               XPM1aEventBlock;
    struct GAS               XPM1bEventBlock;
    struct GAS               XPM1aControlBlock;
    struct GAS               XPM1bControlBlock;
    struct GAS               XPM2ControlBlock;
    struct GAS               XPMTimerBlock;
    struct GAS               XGPE0Block;
    struct GAS               XGPE1Block;
    struct GAS               sleepControlRegister;
    struct GAS               sleepStatusRegister;
    uint64                   hypervisorVendorIdentity;
};
#pragma pack(pop)

#pragma pack(push ,1)
struct FACS {
    uint32 signature;
    uint32 length;
    uint32 hardwareSignature;
    uint32 firmwareWakingVector;
    uint32 globalLock;
    uint32 flags;
    uint64 XFirmwareWakingVector;
    uint8  version;
    byte   reserved1[3];
    uint32 OSPMFlags;
    byte   reserved2[24];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DSDT {
    struct DescriptionHeader header;
    void*                    definitionBlock;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SSDT {
    struct DescriptionHeader header;
    void* definitionBlock;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MADT {
    struct DescriptionHeader header;
    uint32                   localInterruptControllerAddress;
    uint32                   flags;
    byte                     interruptControllerStructure[1];
};
#pragma pack(pop)

struct FADT* GetFADT(void);

struct MADT* GetMADT(void);

void ParseACPITable(
  const struct RSDP* RSDP);

static void* FindDescriptionTable(
  const struct XSDT* XSDT,
  const byte* signature);

static bool CheckRSDP(
  const struct RSDP* RSDP);

static bool CheckDescriptionHeader(
  const struct DescriptionHeader* header,
  const byte*                     signature);

//#pragma pack(push, 1)
//struct NMISourceStructure {
//    uint8  type;
//    uint8  length;
//    uint16 flags;
//    uint32 globalSystemInterrupt;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct LocalAPICNMIStructure {
//    uint8  type;
//    uint8  length;
//    uint8  ACPIProcessorUID;
//    uint16 flags;
//    uint8  localAPICLINTNumber;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct LocalAPICAddressOverrideStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved[2];
//    uint64 localAPICAddress;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct IOSAPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  IOAPICID;
//    byte   reserved;
//    uint32 globalSystemInterruptBase;
//    uint64 IOSAPICAddress;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ProcessorLocalSAPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  ACPIProcessorID;
//    uint8  localSAPICID;
//    uint8  localSAPICEID;
//    byte   reserved[3];
//    uint32 flags;
//    uint32 ACPIProcessorUIDValue;
//    void*  ACPIProcessorUIDString;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct PlatformInterruptSourceStructure {
//    uint8  type;
//    uint8  length;
//    uint16 flags;
//    uint8  interruptType;
//    uint8  processorID;
//    uint8  processorEID;
//    uint8  IOSAPICVector;
//    uint32 globalSystemInterrupt;
//    uint32 platformInterruptSourceFlags;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ProcessorLocalx2APICStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved[2];
//    uint32 X2APICID;
//    uint32 flags;
//    uint32 ACPIProcessorUID;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct LocalX2APICNMIStructure {
//    uint8  type;
//    uint8  length;
//    uint16 flags;
//    uint32 ACPIProcessorUID;
//    uint8  localX2APICLINTNumber;
//    byte   reserved[3];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GICCStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved1[2];
//    uint32 CPUInterfaceNumber;
//    uint32 ACPIProcessorUID;
//    uint32 flags;
//    uint32 parkingProtocolVersion;
//    uint32 performanceInterruptGSIV;
//    uint64 parkedAddress;
//    uint64 physicalBaseAddress;
//    uint64 GICV;
//    uint64 GICH;
//    uint32 VGICMaintenanceInterrupt;
//    uint64 GICRBaseAddress;
//    uint64 MPIDR;
//    uint8  processorPowerEfficiencyClass;
//    byte   reserved2;
//    uint16 SPEOverflowInterrupt;
//    uint16 TRBEInterrupt;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GICDStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved1[2];
//    uint32 GICID;
//    uint64 physicalBaseAddress;
//    uint32 systemVectorBase;
//    uint8  GICVersion;
//    byte   reserved2[3];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GICMSIFrameStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved[2];
//    uint32 GICMSIFrameID;
//    uint64 physicalBaseAddress;
//    uint32 flags;
//    uint16 SPICount;
//    uint16 SPIBase;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GICRStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved[2];
//    uint64 discoveryRangeBaseAddress;
//    uint32 discoveryRangeLength;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GICITSStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved1[2];
//    uint32 GICITSID;
//    uint64 physicalBaseAddress;
//    byte   reserved2[4];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct CorePICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint32 ACPIProcessorID;
//    uint32 physicalProcessorID;
//    uint32 flags;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct LIOPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint64 baseAddress;
//    uint16 size;
//    uint16 cascadeVector;
//    uint64 cascadeVectorMapping;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct HTPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint64 baseAddress;
//    uint16 size;
//    uint64 cascadeVector;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct EIOPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint8  cascadeVector;
//    uint8  node;
//    uint64 nodeMap;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct MSIPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint64 messageAddress;
//    uint32 start;
//    uint32 count;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct BIOPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint64 baseAddress;
//    uint16 size;
//    uint16 hardwareID;
//    uint16 GSIBase;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct LPCPICStructure {
//    uint8  type;
//    uint8  length;
//    uint8  version;
//    uint64 baseAddress;
//    uint16 size;
//    uint16 cascadeVector;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct SBST {
//    struct DescriptionHeader header;
//    uint32                   warningEnergyLevel;
//    uint32                   lowEnergyLevel;
//    uint32                   criticalEnergyLevel;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ECDT {
//    struct DescriptionHeader header;
//    byte                     ECControl[12];
//    byte                     ECData[12];
//    uint32                   UID;
//    uint8                    GPEBit;
//    void*                    ECID;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct SRAT {
//    struct DescriptionHeader header;
//    byte                     reserved[8];
//    void*                    staticResourceAllocationStructure;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ProcessorLocalAPICSAPICAffinityStructure {
//    uint8  type;
//    uint8  length;
//    uint8  proximityDomain1;
//    uint8  APICID;
//    uint32 flags;
//    uint8  localSAPICEID;
//    byte   proximityDomain2[3];
//    uint32 clockDomain;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct MemoryAffinityStructure {
//    uint8  type;
//    uint8  length;
//    uint32 proximityDomain;
//    byte   reserved1[2];
//    uint32 baseAddressLow;
//    uint32 baseAddressHigh;
//    uint32 lengthLow;
//    uint32 lengthHigh;
//    byte   reserved2[4];
//    uint32 flags;
//    byte   reserved3[8];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ProcessorLocalX2APICAffinityStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved1[2];
//    uint32 proximityDomain;
//    uint32 X2APICID;
//    uint32 flags;
//    uint32 clockDomain;
//    byte   reserved2[4];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GICCAffinityStructure {
//    uint8  type;
//    uint8  length;
//    uint32 proximityDomain;
//    uint32 ACPIProcessorUID;
//    uint32 flags;
//    uint32 clockDomain;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ArchitectureSpecificAffinityStructure {
//    uint8  type;
//    uint8  length;
//    uint32 proximityDomain;
//    byte   reserved[2];
//    uint32 ITSID;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GenericInitiatorAffinityStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved1;
//    uint8  deviceHandleType;
//    uint32 proximityDomain;
//    byte   deviceHandle[16];
//    uint32 flags;
//    byte   reserved2[4];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct ACPIDeviceHandle {
//    uint64 ACPIHID;
//    uint32 ACPIUID;
//    byte   reserved[4];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct PCIDeviceHandle {
//    uint16 PCISegment;
//    uint16 PCIBDFNumber;
//    byte   reserved[12];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct GenericPortAffinityStructure {
//    uint8  type;
//    uint8  length;
//    byte   reserved1;
//    uint8  deviceHandleType;
//    uint32 proximityDomain;
//    byte   deviceHandle[16];
//    uint32 flags;
//    byte   reserved2[4];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct SLIT {
//    struct DescriptionHeader header;
//    uint64                   numberOfSystemLocalities;
//    void**                   entry;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct CPEP {
//    struct DescriptionHeader       header;
//    byte                           reserved[8];
//    struct CPEPProcessorStructure* CPEPProcessorStructure;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct CPEPProcessorStructure  {
//    uint8  type;
//    uint8  length;
//    uint8  processorID;
//    uint8  processorEID;
//    uint32 pollingInterval;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct MSCT {
//    struct DescriptionHeader                           header;
//    uint32                                             offsetToProximityDomainInformationStructure;
//    uint32                                             maximumNumberOfProximityDomains;
//    uint32                                             maximumNumberOfClockDomains;
//    uint64                                             maximumPhysicalAddress;
//    struct MaximumProximityDomainInformationStructure* proximityDomainInformationStructure;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct MaximumProximityDomainInformationStructure {
//    uint8 revision;
//    uint8 length;
//    uint32 proximityDomainRangeLow;
//    uint32 proximityDomainRangeHigh;
//    uint32 maximumProcessorCapacity;
//    uint64 maximumMemoryCapacity;
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct RASF {
//    struct DescriptionHeader header;
//    byte                     RASFPlatformCommunicationChannelIdentifier[12];
//};
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//struct RASFPlatformCommunicationChannelSharedMemoryRegion {
//    uint32 signature;
//    uint16 command;
//    uint16 status;
//    uint16 version;
//    byte   RASCapabilities[16];
//    byte   setRASCapabilities[16];
//    uint16 numberOfRASFParameterBlocks;
//    uint32 setRASCapabilitiesStatus;
//    void*  parameterBlock;
//};
//#pragma pack(pop)