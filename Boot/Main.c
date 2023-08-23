#include "Main.h"

EFI_STATUS EFIAPI Main(
  EFI_HANDLE        imageHandle,
  EFI_SYSTEM_TABLE* systemTable) {
    UINTN                           fileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
    UINTN                           kernelFileSize;
    UINT32                          entryAddress;
    EFI_STATUS                      status;

    CHAR8                           memoryMapBuffer[4096 * 4];
    UINT8                           fileInfoBuffer[sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12]; // VLA issue; size of array is equal to fileInfoSize
    UINT8*                          frameBuffer;
    EFI_FILE_INFO*                  fileInfo;
    EFI_FILE_PROTOCOL*              kernelFile;
    EFI_FILE_PROTOCOL*              memoryMapFile;
    EFI_FILE_PROTOCOL*              rootDirectory;
    EFI_GRAPHICS_OUTPUT_PROTOCOL*   GOP;
    EFI_PHYSICAL_ADDRESS            kernelBaseAddress = 0x100000;
    struct MEMORY_MAP               memoryMap = { sizeof(memoryMapBuffer), 0, 0, 0, 0, memoryMapBuffer };
    struct FRAMEBUFFER_CONFIG       FrameBufferConfig;

    InitializeLib(imageHandle, systemTable);

    Print(L"[NOTICE] Hello from DummyOS!\n");

    status = GetMemoryMap(&memoryMap);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to get memory map: %r\n", status);
        while (1);
    }

    status = OpenRootDirectory(
      imageHandle,
      &rootDirectory);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open root directory: %r\n", status);
        while (1);
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
            while (1);
        }

        status = memoryMapFile->Close(memoryMapFile);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to close memory map: %r\n", status);
            while (1);
        }

        Print(L"[NOTICE] Saving memory map is finished.\n");
    }

    status = OpenGOP(
      imageHandle,
      &GOP);

    FrameBufferConfig = (struct FRAMEBUFFER_CONFIG){ GOP->Mode->Info->PixelsPerScanLine, GOP->Mode->Info->HorizontalResolution, GOP->Mode->Info->VerticalResolution, 0, (UINT8*)GOP->Mode->FrameBufferBase };

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open GOP: %r\n", status);
        while (1);
    }

    Print(L"[NOTICE] Resolution: %ux%u, Pixel Format: %s, %u pixels/line\n",
      GOP->Mode->Info->HorizontalResolution,
      GOP->Mode->Info->VerticalResolution,
      GetPixelFormatUnicode(GOP->Mode->Info->PixelFormat),
      GOP->Mode->Info->PixelsPerScanLine);

    Print(L"[NOTICE] Frame Buffer: 0x%0lx - 0x%0lx, Size: %lu bytes\n",
      GOP->Mode->FrameBufferBase,
      GOP->Mode->FrameBufferBase + GOP->Mode->FrameBufferSize,
      GOP->Mode->FrameBufferSize);

    switch (GOP->Mode->Info->PixelFormat) {
        case PixelRedGreenBlueReserved8BitPerColor:
            FrameBufferConfig.PixelFormat = PixelRGBReserved8BitPerColor;
            break;
        case PixelBlueGreenRedReserved8BitPerColor:
            FrameBufferConfig.PixelFormat = PixelBGRReserved8BitPerColor;
            break;
        default:
            Print(L"Unimplemented pixel format : %d\n", GOP->Mode->Info->PixelFormat);
            while (1);
    }

    frameBuffer = (UINT8*)GOP->Mode->FrameBufferBase;

    status = rootDirectory->Open(
      rootDirectory,
      &kernelFile,
      L"\\Kernel.exe",
      EFI_FILE_MODE_READ,
      0);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to open file '\\kernel.exe': %r\n", status);
        while (1);
    }

    status = kernelFile->GetInfo(
      kernelFile,
      &gEfiFileInfoGuid,
      &fileInfoSize,
      fileInfoBuffer);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to get kernel file information: %r\n", status);
        while (1);
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
        while (1);
    }

    status = kernelFile->Read(
      kernelFile, 
      &kernelFileSize, 
      (VOID*)kernelBaseAddress);

    if (EFI_ERROR(status)) {
        Print(L"[ERROR] Failed to read kernel file: %r\n", status);
        while (1);
    }

    Print(L"[NOTICE] Kernel: 0x%0lx (%lu bytes)\n", 
      kernelBaseAddress, kernelFileSize);

    status = BS->ExitBootServices(
      imageHandle, 
      memoryMap.mapKey);

    if (EFI_ERROR(status)) {
        status = GetMemoryMap(&memoryMap);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to get memory map: %r\n", status);
            while (1);
        }

        status = BS->ExitBootServices(
          imageHandle, 
          memoryMap.mapKey);

        if (EFI_ERROR(status)) {
            Print(L"[ERROR] Failed to exit boot service: %r\n", status);
            while (1);
        }
    }

    entryAddress = *(UINT32*)(kernelBaseAddress + 0xA8);

    EntryPointType* entryPoint = (EntryPointType*)(kernelBaseAddress + entryAddress);

    entryPoint(&FrameBufferConfig);

    return EFI_SUCCESS;
}

EFI_STATUS GetMemoryMap(
  struct MEMORY_MAP* map) {
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

EFI_STATUS OpenRootDirectory(
  EFI_HANDLE imageHandle, 
  EFI_FILE_PROTOCOL** rootDirectory) {
    EFI_STATUS                          status;

    EFI_LOADED_IMAGE_PROTOCOL*          loadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*    fileSystem;

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

EFI_STATUS SaveMemoryMap(
  struct MEMORY_MAP* map,
  EFI_FILE_PROTOCOL* file) {
    UINT32                  index;
    UINTN                   length;
    EFI_STATUS              status;

    CHAR8                   buffer[256];
    CHAR8*                  header = "Index, Type, Type(name), PhysicalStart, NumberOfPages, Attribute\n";
    EFI_PHYSICAL_ADDRESS    iterator;
    EFI_MEMORY_DESCRIPTOR* descriptor;

    length = AsciiStrLen(header);
    
    status = file->Write(
      file, 
      &length, 
      header);

    if (EFI_ERROR(status)) {
        return status;
    }

    Print(L"[NOTICE] map->buffer = %08lx, map->map_size = %08lx\n",
      map->buffer, map->mapSize);

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

const CHAR16* GetMemoryTypeUnicode(
  EFI_MEMORY_TYPE type) {
    switch (type) {
        case EfiReservedMemoryType:         return L"EfiReservedMemoryType";
        case EfiLoaderCode:                 return L"EfiLoaderCode";
        case EfiLoaderData:                 return L"EfiLoaderData";
        case EfiBootServicesCode:           return L"EfiBootServicesCode";
        case EfiBootServicesData:           return L"EfiBootServicesData";
        case EfiRuntimeServicesCode:        return L"EfiRuntimeServicesCode";
        case EfiRuntimeServicesData:        return L"EfiRuntimeServicesData";
        case EfiConventionalMemory:         return L"EfiConventionalMemory";
        case EfiUnusableMemory:             return L"EfiUnusableMemory";
        case EfiACPIReclaimMemory:          return L"EfiACPIReclaimMemory";
        case EfiACPIMemoryNVS:              return L"EfiACPIMemoryNVS";
        case EfiMemoryMappedIO:             return L"EfiMemoryMappedIO";
        case EfiMemoryMappedIOPortSpace:    return L"EfiMemoryMappedIOPortSpace";
        case EfiPalCode:                    return L"EfiPalCode";
//        case EfiPersistentMemory:           return L"EfiPersistentMemory";
        case EfiMaxMemoryType:              return L"EfiMaxMemoryType";
        default:                            return L"InvalidMemoryType";
    }
}

EFI_STATUS OpenGOP(
  EFI_HANDLE imageHandle,
  EFI_GRAPHICS_OUTPUT_PROTOCOL** GOP) {
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

    FreePool(GOPHandles);

    return EFI_SUCCESS;
}

const CHAR16* GetPixelFormatUnicode(EFI_GRAPHICS_PIXEL_FORMAT format) {
    switch (format) {
        case PixelRedGreenBlueReserved8BitPerColor:
            return L"PixelRedGreenBlueReserved8BitPerColor";
        case PixelBlueGreenRedReserved8BitPerColor:
            return L"PixelBlueGreenRedReserved8BitPerColor";
        case PixelBitMask:
            return L"PixelBitMask";
        case PixelBltOnly:
            return L"PixelBltOnly";
        case PixelFormatMax:
            return L"PixelFormatMax";
        default:
            return L"InvalidPixelFormat";
    }
}