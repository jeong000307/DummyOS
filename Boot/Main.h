#ifndef __MAIN_H__
#define __MAIN_H__

#include <efi.h>
#include <efiLib.h>

typedef void EntryPointType(const struct FRAMEBUFFER_CONFIG*);

struct MEMORY_MAP {
    UINTN   bufferSize;
    UINTN   mapSize;
    UINTN   mapKey;
    UINTN   descriptorSize;
    UINT32  descriptorVersion;

    VOID*   buffer;
};

enum PixelFormat {
    PixelRGBReserved8BitPerColor,
    PixelBGRReserved8BitPerColor
};

struct FRAMEBUFFER_CONFIG {
    UINT32              pixelsPerScanLine;
    UINT32              horizontalResolution;
    UINT32              verticalResolution;

    enum PIXEL_FORMAT   PixelFormat;
    UINT8*              frameBuffer;
};

EFI_STATUS GetMemoryMap(
    struct MEMORY_MAP* map);

/**
    Gets a memory map.

    @param map  Memory map.

    @retval EFI_SUCCESS             The memory map is gotten.
    @retval EFI_BUFFER_TOO_SMALL    Buffer is smaller than the memory map's size.
    @retval TBD
**/

EFI_STATUS OpenRootDirectory(
    EFI_HANDLE imageHandle,
    EFI_FILE_PROTOCOL** rootDirectory);

/**
    Opens a root directory of boot device.

    @param  ImageHandle     Image handle.
    @param  rootDirectory   Root directory of boot device.

    @retval EFI_SUCCESS The root directory of boot device is opened.
    @retval TBD
**/

EFI_STATUS SaveMemoryMap(
    struct MEMORY_MAP* map,
    EFI_FILE_PROTOCOL* file);

/**
    Saves a memory map.

    @param  map     Memory map.
    @param  file    File to save a memory map.

    @retval EFI_SUCCESS The file is written correctly.
    @retval TBD
**/

const CHAR16* GetMemoryTypeUnicode(
    EFI_MEMORY_TYPE type);

/**
    Gets a memory type in Unicode.

    @param  type    Type of memory.

    @retval L"EfiReservedMemoryType"        Memory is not used.
    @retval L"EfiLoaderCode"                Memory is the code portions of a loaded application.
    @retval L"EfiLoaderData"                Memory is the data portions of a loaded application and the default data allocation type used by an application to allocate pool memory.
    @retval L"EfiBootServicesCode"          Memory is the code portions of a loaded boot services driver.
    @retval L"EfiBootServicesData"          Memory is the data portions of a loaded boot services driver, and the default data allocation type used by a boot services driver to allocate pool memory.
    @retval L"EfiRuntimeServicesCode"       Memory is the code portions of a loaded runtime services driver.
    @retval L"EfiRuntimeServicesData"       Memory is the data portions of a loaded runtime services driver and the default data allocation type used by a runtime services driver to allocate pool memory.
    @retval L"EfiConventionalMemory"        Memory is free.
    @retval L"EfiUnusableMemory"            Memory has errors.
    @retval L"EfiACPIReclaimMemory"         Memory has ACPI tables.
    @retval L"EfiACPIMemoryNVS"             Memory is address space reserved for use by the firmware.
    @retval L"EfiMemoryMappedIO"            Memory is used by system firmware to request that a memory-mapped IO region be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
    @retval L"EfiMemoryMappedIOPortSpace"   Memory is system memory-mapped IO region that is used to translate memory cycles to IO cycles by the processor.
    @retval L"EfiPalCode"                   Memory is address space reserved by the firmware for code that is part of the processor.
    @retval L"EfiPersistentMemory"          Memory is a memory region that operates as EfiConventionalMemory, however it happens to also support byte-addressable non-volatility.
    @retval L"EfiMaxMemoryType"             What is this?
    @retval L"InvalidMemoryType"            Type of memory is unknown.
**/

EFI_STATUS OpenGOP(
  EFI_HANDLE imageHandle, 
  EFI_GRAPHICS_OUTPUT_PROTOCOL** GOP);

/**
    Initializes and opens Graphics Output Protocol.

    @param  imageHandle Image Handle.
    @param  GOP         Protocol of graphics output.

    @retval TBD
**/

const CHAR16* GetPixelFormatUnicode(EFI_GRAPHICS_PIXEL_FORMAT format);

/**
    Gets a format of pixel in Unicode.

    @param  format  Format of pixel.

    @retval L"PixelRedGreenBlueReserved8BitPerColor"    A pixel is 32-bits and byte zero represents red, byte one represents green, byte two represents blue, and byte three is reserved. This is the definition for the physical frame buffer. The byte values for the red, green, and blue components represent the color intensity. This color intensity value range from a minimum intensity of 0 to maximum intensity of 255.
    @retval L"PixelBlueGreenRedReserved8BitPerColor"    A pixel is 32-bits and byte zero represents blue, byte one represents green, byte two represents red, and byte three is reserved. This is the definition for the physical frame buffer. The byte values for the red, green, and blue components represent the color intensity. This color intensity value range from a minimum intensity of 0 to maximum intensity of 255.
    @retval L"PixelBitMask"                             The pixel definition of the physical frame buffer.
    @retval L"PixelBltOnly"                             This mode does not support a physical frame buffer.
    @retval L"PixelFormatMax"                           Valid EFI_GRAPHICS_PIXEL_FORMAT enum values are less than this value.
    @retval L"InvalidPixelFormat"                       The pixel has invalid format.
**/

// Miscellaneous Functions

UINTN AsciiStrLen(CHAR8* string) {
    UINTN length;

    for (length = 0; *string; string += 1, length += 1);

    return length;
}

UINTN AsciiSPrint(CHAR8* buffer, 
  UINTN bufferSize, 
  CONST CHAR8* formatString, 
  ...) {
    va_list marker;
    UINTN   numberOfPrinted;

    va_start(marker, formatString);
    numberOfPrinted = AsciiVSPrint(buffer, bufferSize, formatString, marker);
    va_end(marker);

    return numberOfPrinted;
}

#endif
