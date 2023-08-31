#include "ACPI.h"

static struct FADT FADT;

struct FADT* GetFADT(void) {
    return &FADT;
}

code InitializeACPI(
  const struct XSDP* XSDP) {
    struct XSDT* XSDT = (struct XSDT*)(XSDP->XSDTAddress);

    if (CheckXSDP(XSDP) != true) {
        Assert(ACPI_INVALID_XSDP);
    }

    if (CheckSDT(&XSDT->header, "XSDT") != true) {
        Assert(ACPI_INVALID_XSDT);
    }

    FADT = *(struct FADT*)FindFADT(XSDT);

    return SUCCESS;
}

void WaitMilliSeconds(
  const uint32 milliSeconds) {
    const uint32 PMTimerFreqeuncy = 3579545;
    const bool PMTimer32 = (FADT.flags >> 8) & 1;
    const uint16 PMTimerPort = FADT.PMTimerBlock & 0xffff;
    const uint32 start = IOIn32(PMTimerPort);
    uint32 end = start + PMTimerFreqeuncy * milliSeconds / 1000;

    if (PMTimer32 != true) {
        end &= 0x00ffffffu;
    }

    if (end < start) {
        while(IOIn32(PMTimerPort) >= start);
    }
    while(IOIn32(PMTimerPort) < end);

    return;
}

static void* FindFADT(
  const struct XSDT* XSDT) {
    size length = (XSDT->header.length - sizeof(XSDT->header)) / sizeof(addr);
    size index;
    struct ACPISDTHeader* header = NULL;

    for (index = 0; index < length; ++index) {
        header = (struct ACPISDTHeader*)(XSDT->SDTPointer + index);

        if (CheckSDT(header, "FACP") == true) {
            return (void *)header;
        }
    }

    Assert(ACPI_FADT_NOT_FOUND);
}

static bool CheckXSDP(
  const struct XSDP* XSDP) {
    size index;
    byte sum;

    if (CompareString(XSDP->signiture, "RSD PTR ", 8) != 0) {
        return false;
    }

    if (XSDP->revision < 2) {
        return false;
    }

    sum = 0;
    for (index = 0; index < 20; ++index) sum += *((byte*)XSDP + index);

    if (sum != 0) {
        return false;
    }

    sum = 0;
    for (index = 0; index < 36; ++index)sum += *((byte*)XSDP + index);

    if (sum != 0) {
        return false;
    }

    return true;
}

static bool CheckSDT(
  const struct ACPISDTHeader* header,
  const byte* expectedSignature) {
    size index;
    byte sum;

    if (CompareString(header->signiture, expectedSignature, 4) != 0) {
        return false;
    }

    sum = 0;
    for (index = 0; index < header->length; ++index) sum += *((byte*)header + index);

    if (sum != 0) {
        return false;
    }

    return true;
}