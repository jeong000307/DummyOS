#include "Main.h"

EXTERN EFI_SYSTEM_TABLE*				SystemTable;
EXTERN EFI_GRAPHICS_OUTPUT_PROTOCOL*	GraphicsOutputProtocol;
EXTERN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SimpleFileSystemProtocol;

EFI_MEMORY_DESCRIPTOR	MemoryMapBuffer[256];
SYSTEM_CONFIGURATION	SystemConfiguration;
ENTRY_POINT				EntryPoint;

EFI_STATUS
EFIAPI Main(
	IN	EFI_HANDLE			_ImageHandle,
	IN	EFI_SYSTEM_TABLE*	_SystemTable
)	{

	InitializeEFI(_SystemTable);

	GetSystemConfiguration();

	LoadKernel();

	TransitToKernel(_ImageHandle);

	return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI GetSystemConfiguration(
	VOID
)	{
	UINTN							Index;
	EFI_STATUS						Status;

	SystemConfiguration.MemoryInformation.MapSize = sizeof(MemoryMapBuffer);
	SystemConfiguration.MemoryInformation.MemoryMap = MemoryMapBuffer;

	Status = SystemTable->BootServices->GetMemoryMap(
		&SystemConfiguration.MemoryInformation.MapSize,
		SystemConfiguration.MemoryInformation.MemoryMap,
		&SystemConfiguration.MemoryInformation.MapKey,
		&SystemConfiguration.MemoryInformation.DescriptorSize,
		&SystemConfiguration.MemoryInformation.DescriptorVersion);

	Assert(L"Getting memory map", Status);

	SystemConfiguration.GraphicsInformation.HorizontalResolution = GraphicsOutputProtocol->Mode->Info->HorizontalResolution;
	SystemConfiguration.GraphicsInformation.VerticalResolution = GraphicsOutputProtocol->Mode->Info->VerticalResolution;
	SystemConfiguration.GraphicsInformation.Framebuffer = GraphicsOutputProtocol->Mode->FrameBufferBase;

	switch (GraphicsOutputProtocol->Mode->Info->PixelFormat) {
		case PixelRedGreenBlueReserved8BitPerColor:
			SystemConfiguration.GraphicsInformation.PixelFormat = PixelRedGreenBlueReserved8BitPerColor;
			break;
		case PixelBlueGreenRedReserved8BitPerColor:
			SystemConfiguration.GraphicsInformation.PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
			break;
		default:
			Assert(L"Graphics output protocol does not support RGB Mode", EFI_UNSUPPORTED);
	}

	for (Index = 0; Index < SystemTable->NumberOfTableEntries; ++Index) {
		if (CompareGUID(&(EFI_GUID)EFI_ACPI_TABLE_GUID, &SystemTable->ConfigurationTable[Index].VendorGuid) == TRUE) {
			SystemConfiguration.ACPIInformation = (ROOT_SYSTEM_DESCRIPTOR_POINTER*)SystemTable->ConfigurationTable[Index].VendorTable;
			break;
		}
	}

	if (SystemConfiguration.ACPIInformation == NULL) {
		Assert(L"Founding ACPI Table", EFI_NOT_FOUND);
	}

	return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI LoadKernel(
	VOID
)	{
	UINTN					FileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(UINT16) * 12;
	UINTN					FileSize;
	UINTN					NumberOfPages;
	EFI_STATUS				Status;

	EFI_PHYSICAL_ADDRESS	EntryAddress;
	EFI_PHYSICAL_ADDRESS	FileStartAddress;
	EFI_PHYSICAL_ADDRESS	FileEndAddress;

	UINT8					FileInfoBuffer[sizeof(EFI_FILE_INFO) + sizeof(UINT16) * 12]; // VLA issue; fileInfoSize
	EFI_FILE_INFO*			FileInfo;

	EFI_FILE_PROTOCOL*		Directory;
	EFI_FILE_PROTOCOL*		File;
	VOID*					FileBuffer;

	Status = SimpleFileSystemProtocol->OpenVolume(SimpleFileSystemProtocol, &Directory);
	Assert(L"Opening root directory", Status);

	Status = Directory->Open(Directory, &File, L"\\Kernel.exe", EFI_FILE_MODE_READ, 0);
	Assert(L"Opening file '\\Kernel.exe'", Status);

	Status = File->GetInfo(File, &(EFI_GUID)EFI_FILE_INFO_ID, &FileInfoSize, FileInfoBuffer);
	Assert(L"Getting kernel file information", Status);

	FileInfo = (EFI_FILE_INFO*)FileInfoBuffer;

	FileSize = FileInfo->FileSize;

	Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, FileSize, &FileBuffer);
	Assert(L"Allocating pool", Status);

	Status = File->Read(File, &FileSize, FileBuffer);
	Assert(L"Reading kernel file", Status);

	GetLoadAddressRange(FileBuffer, &FileStartAddress, &FileEndAddress);

	NumberOfPages = (FileEndAddress - FileStartAddress + 0xfff) / 0x1000;

	Status = SystemTable->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, NumberOfPages, &FileStartAddress);
	Assert(L"Allocating pages", Status);

	EntryAddress = *(UINT32*)((INT8*)FileBuffer + 0xE0);

	EntryPoint = (ENTRY_POINT)(FileStartAddress + EntryAddress);

	LoadFileSegment(FileBuffer, FileStartAddress);

	Status = SystemTable->BootServices->FreePool(FileBuffer);
	Assert(L"Freeing pool", Status);

	return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI TransitToKernel(
	IN EFI_HANDLE _ImageHandle
)	{
	EFI_STATUS	Status;

	Status = SystemTable->BootServices->ExitBootServices(_ImageHandle, SystemConfiguration.MemoryInformation.MapKey);

	if (EFI_ERROR(Status)) {
		SystemConfiguration.MemoryInformation.MapSize = sizeof(MemoryMapBuffer);

		Status = SystemTable->BootServices->GetMemoryMap(
			&SystemConfiguration.MemoryInformation.MapSize,
			SystemConfiguration.MemoryInformation.MemoryMap,
			&SystemConfiguration.MemoryInformation.MapKey,
			&SystemConfiguration.MemoryInformation.DescriptorSize,
			&SystemConfiguration.MemoryInformation.DescriptorVersion);

		Status = SystemTable->BootServices->ExitBootServices(_ImageHandle, SystemConfiguration.MemoryInformation.MapKey);

		if(EFI_ERROR(Status))	Assert(L"Exiting boot services", Status);
	}

	EntryPoint(&SystemConfiguration);

	return EFI_SUCCESS;
}

STATIC VOID
EFIAPI GetLoadAddressRange(
	IN VOID*					_FileBuffer,
	OUT EFI_PHYSICAL_ADDRESS*	_Start,
	OUT EFI_PHYSICAL_ADDRESS*	_End
)	{
	UINTN					Index;

	DOSHeader*				FileDOSHeader = (DOSHeader*)_FileBuffer;
	NTHeader*				FileNTHeader = (NTHeader*)((BYTE*)_FileBuffer + FileDOSHeader->e_lfanew);
	EFI_PHYSICAL_ADDRESS	FileSectionHeader = (EFI_PHYSICAL_ADDRESS)_FileBuffer + FileDOSHeader->e_lfanew + sizeof(NTHeader) + FileNTHeader->optionalHeader.NumberOfRvaAndSizes * sizeof(DataDirectory);
	Section*				FileSection;

	if (((EFI_PHYSICAL_ADDRESS)FileSectionHeader - (EFI_PHYSICAL_ADDRESS)_FileBuffer) % 8 != 0) {
		FileSectionHeader += 8 - ((EFI_PHYSICAL_ADDRESS)FileSectionHeader - (EFI_PHYSICAL_ADDRESS)_FileBuffer) % 8;
	}

	*_Start = FileNTHeader->optionalHeader.ImageBase;
	*_End = 0;

	for (Index = 0; Index < FileNTHeader->fileHeader.NumberOfSections; ++Index) {
		FileSection = (Section*)(FileSectionHeader + Index * sizeof(Section));

		*_End = (*_End > FileNTHeader->optionalHeader.ImageBase + FileSection->VirtualAddress + FileSection->VirtualSize) ? *_End : FileNTHeader->optionalHeader.ImageBase + FileSection->VirtualAddress + FileSection->VirtualSize;
	}
}

STATIC VOID
EFIAPI LoadFileSegment(
	IN VOID*				_FileBuffer,
	IN EFI_PHYSICAL_ADDRESS	_FileStartAddress
)	{
	UINTN					Index;

	EFI_PHYSICAL_ADDRESS	Segment;
	DOSHeader*				FileDOSHeader = (DOSHeader*)_FileBuffer;
	NTHeader*				FileNTHeader = (NTHeader*)((BYTE*)_FileBuffer + FileDOSHeader->e_lfanew);
	EFI_PHYSICAL_ADDRESS	FileSectionHeader = (EFI_PHYSICAL_ADDRESS)_FileBuffer + FileDOSHeader->e_lfanew + sizeof(NTHeader) + FileNTHeader->optionalHeader.NumberOfRvaAndSizes * sizeof(DataDirectory);
	Section					FileSection;

	if ((FileSectionHeader - (EFI_PHYSICAL_ADDRESS)_FileBuffer) % 8 != 0) {
		FileSectionHeader += 8 - (FileSectionHeader - (EFI_PHYSICAL_ADDRESS)_FileBuffer) % 8;
	}

	for (Index = 0; Index < FileNTHeader->fileHeader.NumberOfSections; ++Index) {
		FileSection = *((Section*)FileSectionHeader + Index);
		Segment = (EFI_PHYSICAL_ADDRESS)((BYTE*)_FileBuffer + FileSection.PointerToRawData);

		if (FileSection.VirtualSize > FileSection.SizeOfRawData) {
			SystemTable->BootServices->CopyMem((VOID*)(_FileStartAddress + FileSection.VirtualAddress), (VOID*)Segment, FileSection.SizeOfRawData);
			SystemTable->BootServices->SetMem((VOID*)(_FileStartAddress + FileSection.VirtualAddress + FileSection.SizeOfRawData), (UINTN)FileSection.VirtualSize - FileSection.SizeOfRawData, 0);
		}
		else {
			SystemTable->BootServices->CopyMem((VOID*)(_FileStartAddress + FileSection.VirtualAddress), (VOID*)Segment, FileSection.VirtualSize);
			SystemTable->BootServices->SetMem((VOID*)(_FileStartAddress + FileSection.VirtualAddress + FileSection.VirtualSize), (UINTN)FileSection.SizeOfRawData - FileSection.VirtualSize, 0);
		}
	}
}