#include "EFI.h"

EFI_SYSTEM_TABLE*					SystemTable;
EFI_GRAPHICS_OUTPUT_PROTOCOL*		GraphicsOutputProtocol;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*	SimpleFileSystemProtocol;

VOID
EFIAPI InitializeEFI(
	IN	EFI_SYSTEM_TABLE*	_SystemTable
)	{
	SystemTable = _SystemTable;

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
	SystemTable->BootServices->LocateProtocol(&((EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID), NULL, (VOID**)(&GraphicsOutputProtocol));
	SystemTable->BootServices->LocateProtocol(&((EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID), NULL, (VOID**)(&SimpleFileSystemProtocol));

	return;
}

EFI_STATUS
EFIAPI Assert(
	IN			CHAR16*		_String,
	IN CONST	EFI_STATUS	_Status
)	{
	CHAR16* StatusString;

	if (_Status == EFI_SUCCESS) {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[Success] ");
		SystemTable->ConOut->OutputString(SystemTable->ConOut, _String);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\n\r");
		return EFI_SUCCESS;
	}

	if (EFI_ERROR(_Status)) {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[ Error ] ");
	}
	else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[Warning] ");
	}

	SystemTable->ConOut->OutputString(SystemTable->ConOut, _String);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L" : ");

	switch (_Status) {
		case EFI_LOAD_ERROR:			StatusString = L"EFI_LOAD_ERROR"; break;
		case EFI_INVALID_PARAMETER:		StatusString = L"EFI_INVALID_PARAMETER"; break;
		case EFI_UNSUPPORTED:			StatusString = L"EFI_UNSUPPORTED"; break;
		case EFI_BAD_BUFFER_SIZE:		StatusString = L"EFI_BAD_BUFFER_SIZE"; break;
		case EFI_BUFFER_TOO_SMALL:		StatusString = L"EFI_BUFFER_TOO_SMALL"; break;
		case EFI_NOT_READY:				StatusString = L"EFI_NOT_READY"; break;
		case EFI_DEVICE_ERROR:			StatusString = L"EFI_DEVICE_ERROR"; break;
		case EFI_WRITE_PROTECTED:		StatusString = L"EFI_WRITE_PROTECTED"; break;
		case EFI_OUT_OF_RESOURCES:		StatusString = L"EFI_OUT_OF_RESOURCES"; break;
		case EFI_VOLUME_CORRUPTED:		StatusString = L"EFI_VOLUME_CORRUPTED"; break;
		case EFI_VOLUME_FULL:			StatusString = L"EFI_VOLUME_FULL"; break;
		case EFI_NO_MEDIA:				StatusString = L"EFI_NO_MEDIA"; break;
		case EFI_MEDIA_CHANGED:			StatusString = L"EFI_MEDIA_CHANGED"; break;
		case EFI_NOT_FOUND:				StatusString = L"EFI_NOT_FOUND"; break;
		case EFI_ACCESS_DENIED:			StatusString = L"EFI_ACCESS_DENIED"; break;
		case EFI_NO_RESPONSE:			StatusString = L"EFI_NO_RESPONSE"; break;
		case EFI_NO_MAPPING:			StatusString = L"EFI_NO_MAPPING"; break;
		case EFI_TIMEOUT:				StatusString = L"EFI_TIMEOUT"; break;
		case EFI_NOT_STARTED:			StatusString = L"EFI_NOT_STARTED"; break;
		case EFI_ALREADY_STARTED:		StatusString = L"EFI_ALREADY_STARTED"; break;
		case EFI_ABORTED:				StatusString = L"EFI_ABORTED"; break;
		case EFI_ICMP_ERROR:			StatusString = L"EFI_ICMP_ERROR"; break;
		case EFI_TFTP_ERROR:			StatusString = L"EFI_TFTP_ERROR"; break;
		case EFI_PROTOCOL_ERROR:		StatusString = L"EFI_PROTOCOL_ERROR"; break;
		case EFI_INCOMPATIBLE_VERSION:	StatusString = L"EFI_INCOMPATIBLE_VERSION"; break;
		case EFI_SECURITY_VIOLATION:	StatusString = L"EFI_SECURITY_VIOLATION"; break;
		case EFI_CRC_ERROR:				StatusString = L"EFI_CRC_ERROR"; break;
		case EFI_END_OF_MEDIA:			StatusString = L"EFI_END_OF_MEDIA"; break;
		case EFI_END_OF_FILE:			StatusString = L"EFI_END_OF_FILE"; break;
		case EFI_INVALID_LANGUAGE:		StatusString = L"EFI_INVALID_LANGUAGE"; break;
		case EFI_COMPROMISED_DATA:		StatusString = L"EFI_COMPROMISED_DATA"; break;
		case EFI_IP_ADDRESS_CONFLICT:	StatusString = L"EFI_IP_ADDRESS_CONFLICT"; break;
		case EFI_HTTP_ERROR:			StatusString = L"EFI_HTTP_ERROR"; break;
		case EFI_WARN_UNKNOWN_GLYPH:	StatusString = L"EFI_WARN_UNKNOWN_GLYPH"; break;
		case EFI_WARN_DELETE_FAILURE:	StatusString = L"EFI_WARN_DELETE_FAILURE"; break;
		case EFI_WARN_WRITE_FAILURE:	StatusString = L"EFI_WARN_WRITE_FAILURE"; break;
		case EFI_WARN_BUFFER_TOO_SMALL: StatusString = L"EFI_WARN_BUFFER_TOO_SMALL"; break;
		case EFI_WARN_STALE_DATA:		StatusString = L"EFI_WARN_STALE_DATA"; break;
		case EFI_WARN_FILE_SYSTEM:		StatusString = L"EFI_WARN_FILE_SYSTEM"; break;
		case EFI_WARN_RESET_REQUIRED:	StatusString = L"EFI_WARN_RESET_REQUIRED"; break;
		default:						StatusString = L"UNKNOWN_ERROR";
	}

	SystemTable->ConOut->OutputString(SystemTable->ConOut, StatusString);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\n\r");

	if (EFI_ERROR(_Status)) {
		while (1);
	}

	return EFI_SUCCESS;
}

BOOL
EFIAPI CompareGUID(
	IN CONST	EFI_GUID*	_GUID1,
	IN CONST	EFI_GUID*	_GUID2
)	{
	UINTN	index;

	if (_GUID1->Data1 != _GUID2->Data1 OR _GUID1->Data2 != _GUID2->Data2 OR _GUID1->Data3 != _GUID2->Data3)	{
		return FALSE;
	}

	for (index = 0; index < 8; ++index) {
		if (_GUID1->Data4[index] != _GUID2->Data4[index]) {
			return FALSE;
		}
	}

	return TRUE;
}