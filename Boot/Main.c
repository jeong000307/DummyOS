#include "Main.h"

EFI_STATUS Main(
  IN EFI_HANDLE        imageHandle,
  IN EFI_SYSTEM_TABLE* systemTable) {
    UINTN                         fileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
    UINTN                         kernelFileSize;
    UINT32                        entryAddress;
    EFI_STATUS                    status;

    CHAR8                         memoryMapBuffer[4096 * 4];
    UINT8                         fileInfoBuffer[sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12]; // VLA issue; size of array is equal to fileInfoSize
    EFI_FILE_INFO*                fileInfo;
    EFI_FILE_PROTOCOL*            kernelFile;
    EFI_FILE_PROTOCOL*            memoryMapFile;
    EFI_FILE_PROTOCOL*            rootDirectory;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
    EFI_PHYSICAL_ADDRESS          kernelBaseAddress = 0x100000;
    struct MEMORY_MAP             memoryMap = { sizeof(memoryMapBuffer), 0, 0, 0, 0, memoryMapBuffer };
    struct FRAME_BUFFER_CONFIG    frameBufferConfig;

    InitializeLib(imageHandle, systemTable);

    status = GetMemoryMap(
      &memoryMap);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to get memory map: %r\n", status);
        Halt();
    }

    status = OpenRootDirectory(
      imageHandle,
      &rootDirectory);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open root directory: %r\n", status);
        Halt();
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
            Halt();
        }

        status = memoryMapFile->Close(
          memoryMapFile);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to close memory map: %r\n", status);
            Halt();
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
        Halt();
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
            Halt();
    }

    status = rootDirectory->Open(
      rootDirectory,
      &kernelFile,
      L"\\Kernel.exe",
      EFI_FILE_MODE_READ,
      0);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open file '\\kernel.exe': %r\n", status);
        Halt();
    }

    status = kernelFile->GetInfo(
      kernelFile,
      &gEfiFileInfoGuid,
      &fileInfoSize,
      fileInfoBuffer);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to get kernel file information: %r\n", status);
        Halt();
    }

    fileInfo = (EFI_FILE_INFO*)fileInfoBuffer;

    kernelFileSize = fileInfo->FileSize;

    status = BS->AllocatePages(
      AllocateAddress,
      EfiLoaderData,
      (kernelFileSize + 0xfff) / 0x1000,
      &kernelBaseAddress);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to allocate pages: %r\n", status);
        Halt();
    }

    status = kernelFile->Read(
      kernelFile,
      &kernelFileSize,
      (VOID*)kernelBaseAddress);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to read kernel file: %r\n", status);
        Halt();
    }

    status = BS->ExitBootServices(
      imageHandle,
      memoryMap.mapKey);

    if (EFI_ERROR(status)) {
        status = GetMemoryMap(&memoryMap);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to get memory map: %r\n", status);
            Halt();
        }

        status = BS->ExitBootServices(
          imageHandle,
          memoryMap.mapKey);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to exit boot service: %r\n", status);
            Halt();
        }
    }

    entryAddress = *(UINT32*)(kernelBaseAddress + 0xA8);

    EntryPointType* entryPoint = (EntryPointType*)(kernelBaseAddress + entryAddress);

    entryPoint(&frameBufferConfig);

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
    UINT32                 index;
    UINTN                  length;
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
    UINTN       numberOfGOPHandles = 0;
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