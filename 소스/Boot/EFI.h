#ifndef __EFI_H__
#define __EFI_H__

#include "Type.h"

#define ENCODE_ERROR(StatusCode)	((EFI_STATUS)(0x8000000000000000ULL | (StatusCode)))
#define ENCODE_WARNING(StatusCode)	((EFI_STATUS)(StatusCode))
#define EFI_ERROR(StatusCode)		(((INTN)(EFI_STATUS)(StatusCode)) < 0)

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
{0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
{0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}}
#define EFI_ACPI_TABLE_GUID \
{0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}}

#define EFI_SUCCESS					(EFI_STATUS)(0)

#define EFI_LOAD_ERROR				ENCODE_ERROR(1)
#define EFI_INVALID_PARAMETER		ENCODE_ERROR(2)
#define EFI_UNSUPPORTED				ENCODE_ERROR(3)
#define EFI_BAD_BUFFER_SIZE			ENCODE_ERROR(4)
#define EFI_BUFFER_TOO_SMALL		ENCODE_ERROR(5)
#define EFI_NOT_READY				ENCODE_ERROR(6)
#define EFI_DEVICE_ERROR			ENCODE_ERROR(7)
#define EFI_WRITE_PROTECTED			ENCODE_ERROR(8)
#define EFI_OUT_OF_RESOURCES		ENCODE_ERROR(9)
#define EFI_VOLUME_CORRUPTED		ENCODE_ERROR(10)
#define EFI_VOLUME_FULL				ENCODE_ERROR(11)
#define EFI_NO_MEDIA				ENCODE_ERROR(12)
#define EFI_MEDIA_CHANGED			ENCODE_ERROR(13)
#define EFI_NOT_FOUND				ENCODE_ERROR(14)
#define EFI_ACCESS_DENIED			ENCODE_ERROR(15)
#define EFI_NO_RESPONSE				ENCODE_ERROR(16)
#define EFI_NO_MAPPING				ENCODE_ERROR(17)
#define EFI_TIMEOUT					ENCODE_ERROR(18)
#define EFI_NOT_STARTED				ENCODE_ERROR(19)
#define EFI_ALREADY_STARTED			ENCODE_ERROR(20)
#define EFI_ABORTED					ENCODE_ERROR(21)
#define EFI_ICMP_ERROR				ENCODE_ERROR(22)
#define EFI_TFTP_ERROR				ENCODE_ERROR(23)
#define EFI_PROTOCOL_ERROR			ENCODE_ERROR(24)
#define EFI_INCOMPATIBLE_VERSION	ENCODE_ERROR(25)
#define EFI_SECURITY_VIOLATION		ENCODE_ERROR(26)
#define EFI_CRC_ERROR				ENCODE_ERROR(27)
#define EFI_END_OF_MEDIA			ENCODE_ERROR(28)
#define EFI_END_OF_FILE				ENCODE_ERROR(31)
#define EFI_INVALID_LANGUAGE		ENCODE_ERROR(32)
#define EFI_COMPROMISED_DATA		ENCODE_ERROR(33)
#define EFI_IP_ADDRESS_CONFLICT		ENCODE_ERROR(34)
#define EFI_HTTP_ERROR				ENCODE_ERROR(35)

#define EFI_WARN_UNKNOWN_GLYPH		ENCODE_WARNING(1)
#define EFI_WARN_DELETE_FAILURE		ENCODE_WARNING(2)
#define EFI_WARN_WRITE_FAILURE		ENCODE_WARNING(3)
#define EFI_WARN_BUFFER_TOO_SMALL	ENCODE_WARNING(4)
#define EFI_WARN_STALE_DATA			ENCODE_WARNING(5)
#define EFI_WARN_FILE_SYSTEM		ENCODE_WARNING(6)
#define EFI_WARN_RESET_REQUIRED		ENCODE_WARNING(7)

#define EFI_FILE_MODE_READ		0x0000000000000001
#define EFI_FILE_MODE_WRITE		0x0000000000000002
#define EFI_FILE_MODE_CREATE	0x8000000000000000
#define EFI_FILE_READ_ONLY		0x0000000000000001
#define EFI_FILE_HIDDEN			0x0000000000000002
#define EFI_FILE_SYSTEM			0x0000000000000004
#define EFI_FILE_RESERVED		0x0000000000000008
#define EFI_FILE_DIRECTORY		0x0000000000000010
#define EFI_FILE_ARCHIVE		0x0000000000000020
#define EFI_FILE_VALID_ATTR		0x0000000000000037

#define EFI_FILE_INFO_ID \
{0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL	EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL	EFI_GRAPHICS_OUTPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;
typedef struct _EFI_FILE_PROTOCOL				EFI_FILE_PROTOCOL;

typedef struct {
	INT32	MaxMode;
	INT32	Mode;
	INT32	Attribute;
	INT32	CursorColumn;
	INT32	CursorRow;
	BOOL	CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef enum {
	PixelRedGreenBlueReserved8BitPerColor,
	PixelBlueGreenRedReserved8BitPerColor,
	PixelBitMask,
	PixelBltOnly,
	PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
	UINT32	RedMask;
	UINT32	GreenMask;
	UINT32	BlueMask;
	UINT32	ReservedMask;
} EFI_PIXEL_BITMASK;

typedef struct {
	UINT32						Version;
	UINT32						HorizontalResolution;
	UINT32						VerticalResolution;
	EFI_GRAPHICS_PIXEL_FORMAT	PixelFormat;
	EFI_PIXEL_BITMASK			PixelInformation;
	UINT32						PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
	UINT32									MaxMode;
	UINT32									Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION*	Info;
	UINTN									SizeOfInfo;
	EFI_PHYSICAL_ADDRESS					FrameBufferBase;
	UINTN									FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct {
	UINT16	Year;
	UINT8	Month;
	UINT8	Day;
	UINT8	Hour;
	UINT8	Minute;
	UINT8	Second;
	UINT8	Pad1;
	UINT32	Nanosecond;
	INT16	TimeZone;
	UINT8	Daylight;
	UINT8	Pad2;
} EFI_TIME;

typedef struct {
	UINT64		Size;
	UINT64		FileSize;
	UINT64		PhysicalSize;
	EFI_TIME	CreateTime;
	EFI_TIME	LastAccessTime;
	EFI_TIME	ModificationTime;
	UINT64		Attribute;
	CHAR16*		FileName;
} EFI_FILE_INFO;

typedef enum {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
	MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiPersistentMemory,
	EfiUnacceptedMemoryType,
	EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef struct {
	UINT32					Type;
	EFI_PHYSICAL_ADDRESS	PhysicalStart;
	EFI_VIRTUAL_ADDRESS		VirtualStart;
	UINT64					NumberOfPages;
	UINT64					Attribute;
} EFI_MEMORY_DESCRIPTOR;

struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_PHYSICAL_ADDRESS	Reserved[3];
};

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	EFI_PHYSICAL_ADDRESS		Reserved1;

	EFI_STATUS					(EFIAPI* OutputString) (
									IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, 
									IN CHAR16* String 
								);

	EFI_PHYSICAL_ADDRESS		Reserved2[4];

	EFI_STATUS					(EFIAPI* ClearScreen) (
									IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This
								);

	EFI_PHYSICAL_ADDRESS		Reserved3[2];

	SIMPLE_TEXT_OUTPUT_MODE*	Mode;
};

struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
	EFI_PHYSICAL_ADDRESS				Reserved[3];

	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE*	Mode;
};

struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
	UINT64		Revision;

	EFI_STATUS	(EFIAPI* OpenVolume) (
					IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* This,
					OUT EFI_FILE_PROTOCOL** Root
				);
};

struct _EFI_FILE_PROTOCOL {
	UINT64					Revision;

	EFI_STATUS				(EFIAPI* Open) (
								IN EFI_FILE_PROTOCOL* This,
								OUT EFI_FILE_PROTOCOL** NewHandle,
								IN CHAR16* FileName,
								IN UINT64 OpenMode,
								IN UINT64 Attributes
							);
	EFI_STATUS				(EFIAPI* Close) (
								IN EFI_FILE_PROTOCOL* This
							);
	EFI_STATUS				(EFIAPI* Delete) (
								IN EFI_FILE_PROTOCOL* This
							);
	EFI_STATUS				(EFIAPI* Read) (
								IN EFI_FILE_PROTOCOL* This,
								IN OUT UINTN* BufferSize,
								OUT VOID* Buffer
							);
	EFI_STATUS				(EFIAPI* Write) (
								IN EFI_FILE_PROTOCOL* This,
								IN OUT UINTN* BufferSize,
								IN VOID* Buffer
							);

	EFI_PHYSICAL_ADDRESS	Reserved1[2];

	EFI_STATUS				(EFIAPI* GetInfo) (
								IN EFI_FILE_PROTOCOL* This,
								IN EFI_GUID* InformationType,
								IN OUT UINTN* BufferSize,
								OUT VOID* Buffer
							);
	EFI_STATUS				(EFIAPI* SetInfo) (
								IN EFI_FILE_PROTOCOL* This,
								IN EFI_GUID* InformationType,
								IN UINTN BufferSize,
								IN VOID* Buffer
							);
	EFI_STATUS				(EFIAPI* Flush) (
								IN EFI_FILE_PROTOCOL* This
							);

	EFI_PHYSICAL_ADDRESS	Reserved2[4];
};

typedef struct {
	UINT64	Signature;
	UINT32	Revision;
	UINT32	HeaderSize;
	UINT32	CRC32;
	UINT32	Reserved;
} EFI_TABLE_HEADER;

typedef struct {
	EFI_TABLE_HEADER		Hdr;

	EFI_PHYSICAL_ADDRESS	Reserved[14];
} EFI_RUNTIME_SERVICES;

typedef struct {
	EFI_TABLE_HEADER		Hdr;

	EFI_PHYSICAL_ADDRESS	Reserved1[2];

	EFI_STATUS				(EFIAPI* AllocatePages) (
								IN EFI_ALLOCATE_TYPE Type,
								IN EFI_MEMORY_TYPE MemoryType,
								IN UINTN Pages,
								IN OUT EFI_PHYSICAL_ADDRESS* Memory
							);
	EFI_STATUS				(EFIAPI* FreePages) (
								IN EFI_PHYSICAL_ADDRESS Memory,
								IN UINTN Pages
							);
	EFI_STATUS				(EFIAPI* GetMemoryMap) (
								IN OUT UINTN* MemoryMapSize,
								OUT EFI_MEMORY_DESCRIPTOR* MemoryMap,
								OUT UINTN* MapKey,
								OUT UINTN* DescriptorSize,
								OUT UINT32* DescriptorVersion
							);
	EFI_STATUS				(EFIAPI* AllocatePool) (
								IN EFI_MEMORY_TYPE PoolType,
								IN UINTN Size,
								OUT VOID** Buffer
							);
	EFI_STATUS				(EFIAPI* FreePool) (
								IN VOID* Buffer
							);

	EFI_PHYSICAL_ADDRESS	Reserved2[19];

	EFI_STATUS				(EFIAPI* ExitBootServices) (
								IN EFI_HANDLE ImageHandle,
								IN UINTN MapKey
							);

	EFI_PHYSICAL_ADDRESS	Reserved3[2];

	EFI_STATUS				(EFIAPI* SetWatchdogTimer) (
								IN UINTN Timeout,
								IN UINT64 WatchdogCode,
								IN UINTN DataSize,
								IN CHAR16* WatchdogData OPTIONAL
							);

	EFI_PHYSICAL_ADDRESS	Reserved4[7];

	EFI_STATUS				(EFIAPI* LocateProtocol) (
								IN EFI_GUID* Protocol,
								IN VOID* Registration OPTIONAL,
								OUT VOID** Interface
							);

	EFI_PHYSICAL_ADDRESS	Reserved5[3];

	VOID					(EFIAPI* CopyMem) (
								IN VOID* Destination,
								IN VOID* Source,
								IN UINTN Length
							);
	VOID					(EFIAPI* SetMem) (
								IN VOID* Buffer,
								IN UINTN Size,
								IN UINT8 Value
							);

	EFI_PHYSICAL_ADDRESS	Reserved6;
} EFI_BOOT_SERVICES;

typedef struct {
	EFI_GUID	VendorGuid;
	VOID*		VendorTable;
} EFI_CONFIGURATION_TABLE;

typedef struct {
	EFI_TABLE_HEADER					Hdr;
	CHAR16*								FirmwareVendor;
	UINT32								FirmwareRevision;
	EFI_HANDLE							ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL*		ConIn;
	EFI_HANDLE							ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*	ConOut;
	EFI_HANDLE							StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*	StdErr;
	EFI_RUNTIME_SERVICES*				RuntimeServices;
	EFI_BOOT_SERVICES*					BootServices;
	UINTN								NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE*			ConfigurationTable;
} EFI_SYSTEM_TABLE;

VOID
EFIAPI InitializeEFI(
	IN	EFI_SYSTEM_TABLE*	_SystemTable
);

EFI_STATUS
EFIAPI Assert(
	IN			CHAR16*		_String,
	IN CONST	EFI_STATUS	_Status
);

BOOL
EFIAPI CompareGUID(
	IN CONST	EFI_GUID*	_GUID1,
	IN CONST	EFI_GUID*	_GUID2
);

#endif