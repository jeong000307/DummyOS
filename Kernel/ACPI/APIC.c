#include "APIC.h"

#define APIC_EXCLUDING_SELF 0x0c0000
#define APIC_TRIGGER_MODE   0x008000
#define APIC_LEVEL          0x004000
#define APIC_DESTINATION_PHYSICAL 0x000000
#define APIC_DELIVERY_STARTUP 0x000600
#define APIC_DELIVERY_INIT 0x000500

static byte LocalAPICID[MAX_CPU_CORE];

byte* GetLocalAPICID(void) {
    return LocalAPICID;
}

void ParseMADT(void) {
    struct MADT* MADT = GetMADT();

    size index = 0;

    struct SystemConfiguration* systemConfiguration = GetSystemConfiguration();
    struct ProcessorLocalAPICStructure* processorLocalAPIC;
    struct InterruptSourceOverrideStructure* interruptSourceOverride;
    struct IOAPICStructure* IOAPIC;

    systemConfiguration->LocalAPICAddress = MADT->localInterruptControllerAddress;
    *(addr*)(systemConfiguration->LocalAPICAddress + 0x300) = APIC_EXCLUDING_SELF | APIC_TRIGGER_MODE | APIC_LEVEL | APIC_DESTINATION_PHYSICAL | APIC_DELIVERY_INIT;
    *(addr*)(systemConfiguration->LocalAPICAddress + 0x300) = APIC_EXCLUDING_SELF | APIC_TRIGGER_MODE | APIC_LEVEL | APIC_DESTINATION_PHYSICAL | APIC_DELIVERY_STARTUP | (Enter32 / 0x1000);

    while (index < MADT->header.length - sizeof(MADT->header) - sizeof(MADT->localInterruptControllerAddress) - sizeof(MADT->flags)) {
        switch (MADT->interruptControllerStructure[index]) {
        case 0x0:
            processorLocalAPIC = (struct ProcessorLocalAPICStructure*)(MADT->interruptControllerStructure + index);

            index += processorLocalAPIC->length;

            if ((processorLocalAPIC->flags & 0b11) == 0b00) {
                continue;
            }
            ++systemConfiguration->numberOfCore;
            LocalAPICID[processorLocalAPIC->ACPIProcessorUID] = processorLocalAPIC->APICID;
            break;
        case 0x1:
            IOAPIC = (struct IOAPICStructure*)(MADT->interruptControllerStructure + index);

            systemConfiguration->IOAPICAddress = (addr)IOAPIC->IOAPICAddress;
            index += IOAPIC->length;
            break;
        case 0x2:
            interruptSourceOverride = (struct InterruptSourceOverrideStructure*)(MADT->interruptControllerStructure + index);

            index += interruptSourceOverride->length;
            break;
        }
    }
}