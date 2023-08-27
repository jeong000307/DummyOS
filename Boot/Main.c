#include "Main.h"

EFI_STATUS Main(
  IN EFI_HANDLE        imageHandle,
  IN EFI_SYSTEM_TABLE* systemTable) {
    UINT32                        entryAddress;
    UINT64                        fileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
    UINT64                        kernelFileSize;
    UINT64                        numberOfPages;
    EFI_STATUS                    status;

    EFI_FILE_INFO*                fileInfo;
    EFI_FILE_PROTOCOL*            kernelFile;
    EFI_FILE_PROTOCOL*            memoryMapFile;
    EFI_FILE_PROTOCOL*            rootDirectory;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
    EFI_PHYSICAL_ADDRESS          kernelStartAddress;
    EFI_PHYSICAL_ADDRESS          kernelEndAddress;

    CHAR8                         memoryMapBuffer[4096 * 4];
    UINT8                         fileInfoBuffer[sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12]; // VLA issue; size of array is equal to fileInfoSize
    VOID*                         kernelBuffer;
    
    struct MEMORY_MAP             memoryMap = { sizeof(memoryMapBuffer), 0, 0, 0, 0, memoryMapBuffer };
    struct FRAME_BUFFER_CONFIG    frameBufferConfig;

    EntryPoint*                   entryPoint;

    InitializeLib(imageHandle, systemTable);

    status = GetMemoryMap(
      &memoryMap);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to get memory map: %r\n", status);
        Pause();
    }

    status = OpenRootDirectory(
      imageHandle,
      &rootDirectory);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open root directory: %r\n", status);
        Pause();
    }

    status = rootDirectory->Open(
      rootDirectory,
      &memoryMapFile,
      L"\\MemoryMap",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
      0);

    if (EFI_ERROR(status)) {
        Print(L"[WARNING] Failed to open file '\\MemoryMap': %r\n", status);
    }
    else {
        status = SaveMemoryMap(
          &memoryMap,
          memoryMapFile);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to save memory map: %r\n", status);
            Pause();
        }

        status = memoryMapFile->Close(
          memoryMapFile);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to close memory map: %r\n", status);
            Pause();
        }
    }

    status = OpenGOP(
      imageHandle,
      &GOP);

    frameBufferConfig = (struct FRAME_BUFFER_CONFIG){ 
      GOP->Mode->Info->PixelsPerScanLine,
      GOP->Mode->Info->HorizontalResolution,
      GOP->Mode->Info->VerticalResolution,
      0,
      (UINT8*)GOP->Mode->FrameBufferBase };

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open GOP: %r\n", status);
        Pause();
    }

    switch (GOP->Mode->Info->PixelFormat) {
        case PixelRedGreenBlueReserved8BitPerColor:
            frameBufferConfig.pixelFormat = pixelRGBReserved8BitPerColor;
            break;
        case PixelBlueGreenRedReserved8BitPerColor:
            frameBufferConfig.pixelFormat = pixelBGRReserved8BitPerColor;
            break;
        default:
            Print(L"[ERROR] Graphics output protocol does not support RGB mode. GOP supports %d mode.\n", GOP->Mode->Info->PixelFormat);
            Pause();
    }

    status = rootDirectory->Open(
      rootDirectory,
      &kernelFile,
      L"\\Kernel.exe",
      EFI_FILE_MODE_READ,
      0);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open file '\\kernel.exe': %r\n", status);
        Pause();
    }

    status = kernelFile->GetInfo(
      kernelFile,
      &gEfiFileInfoGuid,
      &fileInfoSize,
      fileInfoBuffer);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to get kernel file information: %r\n", status);
        Pause();
    }

    fileInfo = (EFI_FILE_INFO*)fileInfoBuffer;

    kernelFileSize = fileInfo->FileSize;

    status = BS->AllocatePool(
      EfiLoaderData, 
      kernelFileSize, 
      &kernelBuffer);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to allocate pool: %r\n", status);
        Pause();
    }

    status = kernelFile->Read(
      kernelFile,
      &kernelFileSize,
      kernelBuffer);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to read kernel file: %r\n", status);
        Pause();
    }

    CalculateLoadAddressRange(kernelBuffer, &kernelStartAddress, &kernelEndAddress);

    numberOfPages = (kernelEndAddress - kernelStartAddress + 0xfff) / 0x1000;

    status = BS->AllocatePages(
      AllocateAddress,
      EfiLoaderData,
      numberOfPages,
      &kernelStartAddress);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to allocate pages: %r\n", status);
        Pause();
    }

    entryAddress = *(UINT32 *)((char *)kernelBuffer + 0xA8);

    entryPoint = (EntryPoint*)(kernelStartAddress + entryAddress);

    LoadKernelSegment(kernelBuffer);

    status = BS->FreePool(kernelBuffer);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to free pool: %r\n", status);
        Pause();
    }

    status = BS->ExitBootServices(
      imageHandle,
      memoryMap.mapKey);

    if (EFI_ERROR(status)) {
        status = GetMemoryMap(&memoryMap);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to get memory map: %r\n", status);
            Pause();
        }

        status = BS->ExitBootServices(
          imageHandle,
          memoryMap.mapKey);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to exit boot service: %r\n", status);
            Pause();
        }
    }

    entryPoint(&frameBufferConfig, &memoryMap);

    return EFI_SUCCESS;
}

static EFI_STATUS GetMemoryMap(
  OUT struct MEMORY_MAP* map) {
    if (map->buffer == NULL) {
        return EFI_BUFFER_TOO_SMALL;
    }

    map->mapSize = map->bufferSize;

    return BS->GetMemoryMap(
      &map->mapSize,
      (EFI_MEMORY_DESCRIPTOR*)map->buffer,
      &map->mapKey,
      &map->descriptorSize,
      &map->descriptorVersion);
}

static EFI_STATUS OpenRootDirectory(
  IN  EFI_HANDLE          imageHandle,
  OUT EFI_FILE_PROTOCOL** rootDirectory) {
    EFI_STATUS                       status;

    EFI_LOADED_IMAGE_PROTOCOL*       loadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem;

    status = BS->LocateProtocol(
      &gEfiLoadedImageProtocolGuid,
      NULL,
      (VOID**)&loadedImage);

    if (EFI_ERROR(status)) {
        return status;
    }

    status = BS->LocateProtocol(
      &gEfiSimpleFileSystemProtocolGuid,
      NULL,
      (VOID**)&fileSystem);

    if (EFI_ERROR(status)) {
        return status;
    }

    return fileSystem->OpenVolume(
      fileSystem,
      rootDirectory);
}

static EFI_STATUS SaveMemoryMap(
  IN  struct MEMORY_MAP* map,
  OUT EFI_FILE_PROTOCOL* file) {
    UINT64                 length;
    UINT32                 index;
    EFI_STATUS             status;

    CHAR8                  buffer[256];
    CHAR8*                 header = "Index, Type, Type(name), PhysicalStart, NumberOfPages, Attribute\n";
    EFI_PHYSICAL_ADDRESS   iterator;
    EFI_MEMORY_DESCRIPTOR* descriptor;

    length = AsciiStrLen(
      header);

    status = file->Write(
      file,
      &length,
      header);

    if (EFI_ERROR(status)) {
        return status;
    }

    for (iterator = (EFI_PHYSICAL_ADDRESS)map->buffer, index = 0;
      iterator < (EFI_PHYSICAL_ADDRESS)map->buffer + map->mapSize;
      iterator += map->descriptorSize, ++index) {
        descriptor = (EFI_MEMORY_DESCRIPTOR*)iterator;

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

        status = file->Write(
          file,
          &length,
          buffer);

        if (EFI_ERROR(status)) {
            return status;
        }
    }

    return EFI_SUCCESS;
}

static CONST CHAR16* GetMemoryTypeUnicode(
  IN EFI_MEMORY_TYPE type) {
    switch (type) {
        case EfiReservedMemoryType:      return L"EfiReservedMemoryType";
        case EfiLoaderCode:              return L"EfiLoaderCode";
        case EfiLoaderData:              return L"EfiLoaderData";
        case EfiBootServicesCode:        return L"EfiBootServicesCode";
        case EfiBootServicesData:        return L"EfiBootServicesData";
        case EfiRuntimeServicesCode:     return L"EfiRuntimeServicesCode";
        case EfiRuntimeServicesData:     return L"EfiRuntimeServicesData";
        case EfiConventionalMemory:      return L"EfiConventionalMemory";
        case EfiUnusableMemory:          return L"EfiUnusableMemory";
        case EfiACPIReclaimMemory:       return L"EfiACPIReclaimMemory";
        case EfiACPIMemoryNVS:           return L"EfiACPIMemoryNVS";
        case EfiMemoryMappedIO:          return L"EfiMemoryMappedIO";
        case EfiMemoryMappedIOPortSpace: return L"EfiMemoryMappedIOPortSpace";
        case EfiPalCode:                 return L"EfiPalCode";
        case EfiMaxMemoryType:           return L"EfiMaxMemoryType";
        default:                         return L"InvalidMemoryType";
    }
}

static EFI_STATUS OpenGOP(
  IN EFI_HANDLE                     imageHandle,
  OUT EFI_GRAPHICS_OUTPUT_PROTOCOL** GOP) {
    UINT64      numberOfGOPHandles = 0;
    EFI_STATUS  status;

    EFI_HANDLE* GOPHandles = NULL;

    status = BS->LocateHandleBuffer(
      ByProtocol,
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      &numberOfGOPHandles,
      &GOPHandles);

    if (EFI_ERROR(status)) {
        return status;
    }

    status = BS->OpenProtocol(
      GOPHandles[0],
      &gEfiGraphicsOutputProtocolGuid,
      (VOID**)GOP,
      imageHandle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    if (EFI_ERROR(status)) {
        return status;
    }

    FreePool(
      GOPHandles);

    return EFI_SUCCESS;
}

static CONST CHAR16* GetPixelFormatUnicode(
  IN EFI_GRAPHICS_PIXEL_FORMAT format) {
    switch (format) {
        case PixelRedGreenBlueReserved8BitPerColor: return L"PixelRedGreenBlueReserved8BitPerColor";
        case PixelBlueGreenRedReserved8BitPerColor: return L"PixelBlueGreenRedReserved8BitPerColor";
        case PixelBitMask:                          return L"PixelBitMask";
        case PixelBltOnly:                          return L"PixelBltOnly";
        case PixelFormatMax:                        return L"PixelFormatMax";
        default:                                    return L"InvalidPixelFormat";
    }
}

static void CalculateLoadAddressRange(
  IN VOID* kernelBuffer,
  OUT EFI_PHYSICAL_ADDRESS* start,
  OUT EFI_PHYSICAL_ADDRESS* end) {
    UINT64   index;

    DOSHeader* kernelDOSHeader = (DOSHeader*)kernelBuffer;
    NTHeader* kernelNTHeader = (NTHeader *)((char *)kernelBuffer + kernelDOSHeader->e_lfanew);
    EFI_PHYSICAL_ADDRESS kernelSectionHeader = (EFI_PHYSICAL_ADDRESS)kernelBuffer + kernelDOSHeader->e_lfanew + sizeof(NTHeader) + kernelNTHeader->optionalHeader.NumberOfRvaAndSizes * sizeof(DataDirectory);
    Section* section;

    if (((EFI_PHYSICAL_ADDRESS)kernelSectionHeader - (EFI_PHYSICAL_ADDRESS)kernelBuffer) % 8 != 0) {
        kernelSectionHeader += 8 - ((EFI_PHYSICAL_ADDRESS)kernelSectionHeader - (EFI_PHYSICAL_ADDRESS)kernelBuffer) % 8;
    }

    *start = kernelNTHeader->optionalHeader.ImageBase;
    *end = 0;

    for (index = 0; index < kernelNTHeader->fileHeader.NumberOfSections; ++index) {
        section = (Section *)(kernelSectionHeader + index * sizeof(Section));

        *end = (*end > kernelNTHeader->optionalHeader.ImageBase + section->VirtualAddress + section->VirtualSize) ? *end: kernelNTHeader->optionalHeader.ImageBase + section->VirtualAddress + section->VirtualSize;
    }
}

static void LoadKernelSegment(
  IN VOID* kernelBuffer) {
    UINT64 index;
    EFI_PHYSICAL_ADDRESS segment;
    INT64 remain;
    
    DOSHeader* kernelDOSHeader = (DOSHeader*)kernelBuffer;
    NTHeader* kernelNTHeader = (NTHeader *)((char *)kernelBuffer + kernelDOSHeader->e_lfanew);
    EFI_PHYSICAL_ADDRESS kernelSectionHeader = (EFI_PHYSICAL_ADDRESS)kernelBuffer + kernelDOSHeader->e_lfanew + sizeof(NTHeader) + kernelNTHeader->optionalHeader.NumberOfRvaAndSizes * sizeof(DataDirectory);
    Section section;

    if ((kernelSectionHeader - (EFI_PHYSICAL_ADDRESS)kernelBuffer) % 8 != 0) {
        kernelSectionHeader += 8 - (kernelSectionHeader - (EFI_PHYSICAL_ADDRESS)kernelBuffer) % 8;
    }

    for(index = 0; index < kernelNTHeader->fileHeader.NumberOfSections; ++index) {
        section = *((Section *)kernelSectionHeader + index);
        segment = (EFI_PHYSICAL_ADDRESS)((char *)kernelBuffer + section.PointerToRawData);

        if (section.VirtualSize > section.SizeOfRawData) {
            CopyMem(
                (VOID*)(kernelNTHeader->optionalHeader.ImageBase + section.VirtualAddress),
                (VOID*)segment,
                section.SizeOfRawData);

            SetMem(
              (VOID*)(kernelNTHeader->optionalHeader.ImageBase + section.VirtualAddress + section.SizeOfRawData),
              section.VirtualSize - section.SizeOfRawData,
              0);
        } else {
            CopyMem(
                (VOID*)(kernelNTHeader->optionalHeader.ImageBase + section.VirtualAddress),
                (VOID*)segment,
                section.VirtualSize);

            SetMem(
              (VOID*)(kernelNTHeader->optionalHeader.ImageBase + section.VirtualAddress + section.VirtualSize),
              section.SizeOfRawData - section.VirtualSize,
              0);
        }
    }
}