#pragma once

#include "ACPI.h"
#include "../SystemConfiguration.h"
#include "../Timer.h"
#include "../Type.h"

#define MAX_CPU_CORE 16

#pragma pack(push, 1)
struct ProcessorLocalAPICStructure {
    uint8  type;
    uint8  length;
    uint8  ACPIProcessorUID;
    uint8  APICID;
    uint32 flags;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct IOAPICStructure {
    uint8  type;
    uint8  length;
    uint8  IOAPICID;
    byte   reserved;
    uint32 IOAPICAddress;
    uint32 globalSystemInterruptBase;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct InterruptSourceOverrideStructure {
    uint8  type;
    uint8  length;
    uint8  bus;
    uint8  source;
    uint32 globalSystemInterrupt;
    uint16 flags;
};
#pragma pack(pop)

byte* GetLocalAPICID(void);

void EnableApplicationProcessor(void);

void ParseMADT(void);