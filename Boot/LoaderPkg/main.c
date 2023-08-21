#include <main.h>

EFI_STATUS EFIAPI UefiMain(
  EFI_HANDLE		imageHandle,
  EFI_SYSTEM_TABLE*	systemTable) {
	CHAR8 memoryMapBuffer[4096 * 4];
	EFI_FILE_PROTOCOL* rootDirectory;
	EFI_FILE_PROTOCOL* memoryMapFile;
	struct MEMORY_MAP memoryMap = { sizeof(memoryMapBuffer), memoryMapBuffer, 0, 0, 0, 0 };

	Print(L"Hello, World!\n");

	GetMemoryMap(&memoryMap);

	OpenRootDirectory(
	  imageHandle, 
	  &rootDirectory);

	rootDirectory->Open(
	  rootDirectory, 
	  &memoryMapFile,
	  L"\\MemoryMap",
	  EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 
	  0);

	SaveMemoryMap(
	  &memoryMap, 
	  memoryMapFile);

	memoryMapFile->Close(memoryMapFile);

	Print(L"All Done.\n");

	return EFI_SUCCESS;
}

EFI_STATUS GetMemoryMap(
  struct MEMORY_MAP* map) {
	if (map->buffer == NULL) {
		return EFI_BUFFER_TOO_SMALL;
	}

	map->mapSize = map->bufferSize;

	return gBS->GetMemoryMap(
	  &map->mapSize,
	  (EFI_MEMORY_DESCRIPTOR*)map->buffer,
	  &map->mapKey,
	  &map->descriptorSize,
	  &map->descriptorVersion);
}

EFI_STATUS OpenRootDirectory(
  EFI_HANDLE imageHandle, 
  EFI_FILE_PROTOCOL** rootDirectory) {
	EFI_LOADED_IMAGE_PROTOCOL*			loadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*	fileSystem;

	gBS->OpenProtocol(
	  imageHandle,
	  &gEfiLoadedImageProtocolGuid,
	  (VOID**)&loadedImage,
	  imageHandle,
	  NULL,
	  EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

	gBS->OpenProtocol(
	  loadedImage->DeviceHandle,
	  &gEfiSimpleFileSystemProtocolGuid,
	  (VOID**)&fileSystem,
	  imageHandle,
	  NULL,
	  EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

	fileSystem->OpenVolume(
	  fileSystem, 
	  rootDirectory);

	return EFI_SUCCESS;
}

EFI_STATUS SaveMemoryMap(
  struct MEMORY_MAP* map,
  EFI_FILE_PROTOCOL* file) {
	INT32					index;
	CHAR8					buffer[256];
	CHAR8*					header = "Index, Type, Type(name), PhysicalStart, NumberOfPages, Attribute\n";
	UINTN					length = AsciiStrLen(header);
	EFI_PHYSICAL_ADDRESS	iterator;
	
	file->Write(
	  file, 
	  &length, 
	  header);

	Print(
	  L"map->buffer = %08lx, map->map_size = %08lx\n", 
	  map->buffer, 
	  map->mapSize);

	for (iterator = (EFI_PHYSICAL_ADDRESS)map->buffer, index = 0; 
	  iterator < (EFI_PHYSICAL_ADDRESS)map->buffer + map->mapSize; 
	  iterator += map->descriptorSize, ++index) {
		EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)iterator;

		length = AsciiSPrint(
		  buffer, 
		  sizeof(buffer), 
		  "%u, %x, %-ls, %08lx, %lx, %lx\n", 
		  index, 
		  descriptor->Type, 
		  GetMemoryTypeUnicode(descriptor->Type), 
		  descriptor->PhysicalStart,
		  descriptor->NumberOfPages, 
		  descriptor->Attribute & 0xfffflu);

		file->Write(file, 
		  &length, 
		  buffer);
	}

	return EFI_SUCCESS;
}

const CHAR16* GetMemoryTypeUnicode(
  EFI_MEMORY_TYPE type) {
	switch (type) {
		case EfiReservedMemoryType:			return L"EfiReservedMemoryType";
		case EfiLoaderCode:					return L"EfiLoaderCode";
		case EfiLoaderData:					return L"EfiLoaderData";
		case EfiBootServicesCode:			return L"EfiBootServicesCode";
		case EfiBootServicesData:			return L"EfiBootServicesData";
		case EfiRuntimeServicesCode:		return L"EfiRuntimeServicesCode";
		case EfiRuntimeServicesData:		return L"EfiRuntimeServicesData";
		case EfiConventionalMemory:			return L"EfiConventionalMemory";
		case EfiUnusableMemory:				return L"EfiUnusableMemory";
		case EfiACPIReclaimMemory:			return L"EfiACPIReclaimMemory";
		case EfiACPIMemoryNVS:				return L"EfiACPIMemoryNVS";
		case EfiMemoryMappedIO:				return L"EfiMemoryMappedIO";
		case EfiMemoryMappedIOPortSpace:	return L"EfiMemoryMappedIOPortSpace";
		case EfiPalCode:					return L"EfiPalCode";
		case EfiPersistentMemory:			return L"EfiPersistentMemory";
		case EfiMaxMemoryType:				return L"EfiMaxMemoryType";
		default:							return L"InvalidMemoryType";
	}
}