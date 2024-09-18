#include "ACPI.h"

STATIC	FIXED_ACPI_DESCRIPTION_TABLE*	FADT;

FIXED_ACPI_DESCRIPTION_TABLE*
DUMMYAPI GetFADT(
	VOID
)	{
	return FADT;
}

STATUS
DUMMYAPI ParseACPITable(
	IN CONST	ROOT_SYSTEM_DESCRIPTOR_POINTER*	_RSDP
)   {
	STATUS								status;
	EXTENDED_SYSTEM_DESCRIPTION_TABLE*	XSDT = (EXTENDED_SYSTEM_DESCRIPTION_TABLE*)(_RSDP->XSDTAddress);

	if (CheckRSDP(_RSDP) != TRUE) {
		return ERR_COMPROMISED_DATA;
	}

	if (CheckDescriptionHeader(&XSDT->header, "XSDT") != TRUE) {
		return ERR_COMPROMISED_DATA;
	}

	status = FindDescriptionTable(XSDT, "FACP", &FADT);

	if (ERROR(status)) {
		return ERR_COMPROMISED_DATA;
	}

	return SUCCESS;
}

STATIC STATUS
DUMMYAPI FindDescriptionTable(
	IN CONST	EXTENDED_SYSTEM_DESCRIPTION_TABLE*	_XSDT,
	IN CONST	BYTE*                               _signature,
	OUT			FIXED_ACPI_DESCRIPTION_TABLE**		_FADT
)   {
	UINTN               length = (_XSDT->header.length - sizeof(_XSDT->header)) / sizeof(UINTN);
	UINTN               index;

	DESCRIPTION_HEADER* header = NULL;

	for (index = 0; index < length; ++index) {
		header = (DESCRIPTION_HEADER*)_XSDT->entry[index];
		
		if (CheckDescriptionHeader(header, _signature) == TRUE) {
			*_FADT = (FIXED_ACPI_DESCRIPTION_TABLE*)header;

			return SUCCESS;
		}
	}

	return ERR_NOT_FOUND;
}

STATIC BOOLEAN
DUMMYAPI CheckRSDP(
	IN CONST	ROOT_SYSTEM_DESCRIPTOR_POINTER* _RSDP
)   {
	UINTN	index;
	BYTE	sum;

	if (CompareString(_RSDP->signiture, "RSD PTR ", 8) != 0) {
		return FALSE;
	}

	if (_RSDP->revision < 2) {
		return FALSE;
	}

	for (index = 0, sum = 0; index < 20; ++index) {
		sum += *((BYTE*)_RSDP + index);
	}

	if (sum != 0) {
		return FALSE;
	}

	for (index = 0, sum = 0; index < 36; ++index) {
		sum += *((BYTE*)_RSDP + index);
	}

	if (sum != 0) {
		return FALSE;
	}

	return TRUE;
}

STATIC BOOLEAN
DUMMYAPI CheckDescriptionHeader(
	IN CONST	DESCRIPTION_HEADER* _header,
	IN CONST	BYTE*				_signature
)	{
	UINTN	index;
	BYTE	sum;

	if (CompareString(_header->signiture, _signature, 4) != 0) {
		return FALSE;
	}

	for (index = 0, sum = 0; index < _header->length; ++index) {
		sum += *((BYTE*)_header + index);
	}

	if (sum != 0) {
		return FALSE;
	}

	return TRUE;
}