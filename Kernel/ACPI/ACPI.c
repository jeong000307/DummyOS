#include "ACPI.h"

static struct FADT* FADT;
static struct MADT* MADT;

struct FADT* GetFADT(void) {
    return FADT;
}

struct MADT* GetMADT(void) {
    return MADT;
}

void ParseACPITable(
  const struct RSDP* RSDP) {
    struct SystemConfiguration* systemConfiguration = GetSystemConfiguration();
    struct XSDT* XSDT = (struct XSDT*)(RSDP->XSDTAddress);

    if (CheckRSDP(RSDP) != true) {
        Assert(ACPI_INVALID_RSDP);
    }

    if (CheckDescriptionHeader(&XSDT->header, "XSDT") != true) {
        Assert(ACPI_INVALID_XSDT);
    }

    FADT = (struct FADT*)FindDescriptionTable(XSDT, "FACP");

    if (FADT == NULL) {
        Assert(ACPI_INVALID_FADT);
    }

    MADT = (struct MADT*)FindDescriptionTable(XSDT, "APIC");
}

static void* FindDescriptionTable(
  const struct XSDT* XSDT,
  const byte* signature) {
    size length = (XSDT->header.length - sizeof(XSDT->header)) / sizeof(addr);
    size index;

    struct DescriptionHeader* header = NULL;

    for (index = 0; index < length; ++index) {
        header = XSDT->entry[index];
        
        if (CheckDescriptionHeader(header, signature) == true) {
            return (void*)header;
        }
    }

    return NULL;
}

static bool CheckRSDP(
  const struct RSDP* RSDP) {
    size index;
    byte sum;

    if (CompareString(RSDP->signiture, "RSD PTR ", 8) != 0) {
        return false;
    }

    if (RSDP->revision < 2) {
        return false;
    }

    sum = 0;
    for (index = 0; index < 20; ++index) sum += *((byte*)RSDP + index);

    if (sum != 0) {
        return false;
    }

    sum = 0;
    for (index = 0; index < 36; ++index) sum += *((byte*)RSDP + index);

    if (sum != 0) {
        return false;
    }

    return true;
}

static bool CheckDescriptionHeader(
  const struct DescriptionHeader* header,
  const byte* signature) {
    size index;
    byte sum;

    if (CompareString(header->signiture, signature, 4) != 0) {
        return false;
    }

    sum = 0;
    for (index = 0; index < header->length; ++index) sum += *((byte*)header + index);

    if (sum != 0) {
        return false;
    }

    return true;
}