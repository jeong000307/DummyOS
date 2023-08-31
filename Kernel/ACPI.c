#include "ACPI.h"
#include "Console.h"

struct XSDP ACPITable;

struct FADT* FADT;

struct XSDP* GetACPITable(void) {
    return &ACPITable;
}

code InitializeACPI(
  struct XSDP* XSDP) {
    size         index;
    struct XSDT* XSDT = (addr)(XSDP->XSDTAddress);
    struct FADT* FADT = NULL;

    if (CheckXSDP(XSDP) != true) {
        return ACPI_ERROR;
    }

    if (CheckSDT(&XSDT->header, "XSDT") != true) {
        return ACPI_ERROR;
    }

    FADT = (struct FADT*)FindFADT(XSDT);

    if (FADT == NULL) {
        return ACPI_ERROR;
    }

    return SUCCESS;
}

void WaitMilliSeconds(uint32 milliSeconds) {
    const size PMTimerFreqeuncy = 3579545;
    const bool PMTimer32 = (FADT->flags >> 8) & 1;
    const uint32 start = IOIn32(FADT->PMTimerBlock);
    uint32 end = start + PMTimerFreqeuncy * milliSeconds / 1000;

    if (PMTimer32 != true) {
        end &= 0x00ffffffu;
    }

    if (end < start) {
        while(IOIn32(FADT->PMTimerBlock) >= start);
    }
    while(IOIn32(FADT->PMTimerBlock) < end);

    return;
}

void* FindFADT(struct XSDT* XSDT) {
    size length = (XSDT->header.length - sizeof(XSDT->header)) / sizeof(addr);
    size index;
    struct ACPI_SDT_HEADER* header = NULL;
    CONSOLE* con = GetSystemConsole();

    for (index = 0; index < length; ++index) {
        header = (struct ACPI_SDT_HEADER*)(XSDT->OtherSDTPointer + 8 * index);

        if (CheckSDT(header, "FACP")) {
            return (void *)header;
        }
    }

    return NULL;
}

bool CheckXSDP(
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

bool CheckSDT(
  const struct ACPI_SDT_HEADER* header,
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