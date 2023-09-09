#pragma once

#include "efi.h"
#include "efilib.h"

#include "AssemblyFunction.h"
#include "PE.h"

#define MAX_CPU_CORE 16

enum PIXEL_FORMAT {
    pixelRGBReserved8BitPerColor,
    pixelBGRReserved8BitPerColor
};

struct FRAME_BUFFER_CONFIGURATION {
    UINT32            horizontalResolution;
    UINT32            verticalResolution;

    enum PIXEL_FORMAT pixelFormat;
    UINT8*            frameBuffer;
};

struct MEMORY_MAP {
    UINTN  bufferSize;
    UINTN  mapSize;
    UINTN  mapKey;
    UINTN  descriptorSize;
    UINT32 descriptorVersion;

    VOID*  buffer;
};

typedef void EntryPoint(
  IN const struct FRAME_BUFFER_CONFIGURATION*,
  IN const struct MEMORY_MAP*,
  IN const VOID*);

static EFI_STATUS GetMemoryMap(
  OUT struct MEMORY_MAP* memoryMap);

/**
    Gets a memory map.

    @param  map                   Memory map.

    @retval EFI_SUCCESS           The memory map was returned in the map.
    @retval EFI_BUFFER_TOO_SMALL  Buffer in the map was too small. Buffer size needed to hold the memory map is returned in size variable of map.
    @retval EFI_INVALID_PARAMETER Size of map is NULL or buffer of map is not too small but map is NULL.
    @retval TBD
**/

static EFI_STATUS OpenRootDirectory(
  OUT EFI_FILE_PROTOCOL** rootDirectory);

/**
    Opens a root directory of boot device.

    @param  ImageHandle     Image handle.
    @param  rootDirectory   Root directory of the file volume.

    @retval EFI_SUCCESS          The file volume was opened.
    @retval EFI_UNSUPPORTED      The volume does not support the requested file system type.
    @retval EFI_NO_MEDIA         The device has no medium.
    @retval EFI_DEVICE_ERROR     The device reported an error.
    @retval EFI_VOLUME_CORRUPTED The file system structrues are corrupted.
    @retval EFI_ACCESS_DENIED    The service denied access to the file.
    @retval EFI_OUT_OF_RESOURCES The file volume was not opened.
    @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported. Any existing file handles for this voulme are no longer valid. To access the files on the new medium, the volume must be reopened with OpenVolume().
**/

static EFI_STATUS SaveMemoryMap(
  IN struct MEMORY_MAP*  map,
  OUT EFI_FILE_PROTOCOL* file);

/**
    Saves a memory map.

    @param  map                  Memory map.
    @param  file                 File to save a memory map.

    @retval EFI_SUCCESS          The data was written.
    @retval EFI_UNSUPPORT        Writes to open directory files are not supported.
    @retval EFI_NO_MEDIA         The device has no medium.
    @retval EFI_DEVICE_ERROR     The device reported an error or An attempt was made to write to a deleted file.
    @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted.
    @retval EFI_WRITE_PROTECTED  The file or medium is write-protected.
    @retval EFI_ACCESS_DENIED    The file was opened read only.
    @retval EFI_VOLUME_FULL      The volume is full.
**/

static CONST CHAR16* GetMemoryTypeUnicode(
  IN EFI_MEMORY_TYPE type);

/**
    Gets a memory type in Unicode.

    @param  type                          Type of memory.

    @retval L"EfiReservedMemoryType"      Memory is not used.
    @retval L"EfiLoaderCode"              Memory is the code portions of a loaded application.
    @retval L"EfiLoaderData"              Memory is the data portions of a loaded application and the default data allocation type used by an application to allocate pool memory.
    @retval L"EfiBootServicesCode"        Memory is the code portions of a loaded boot services driver.
    @retval L"EfiBootServicesData"        Memory is the data portions of a loaded boot services driver, and the default data allocation type used by a boot services driver to allocate pool memory.
    @retval L"EfiRuntimeServicesCode"     Memory is the code portions of a loaded runtime services driver.
    @retval L"EfiRuntimeServicesData"     Memory is the data portions of a loaded runtime services driver and the default data allocation type used by a runtime services driver to allocate pool memory.
    @retval L"EfiConventionalMemory"      Memory is free.
    @retval L"EfiUnusableMemory"          Memory has errors.
    @retval L"EfiACPIReclaimMemory"       Memory has ACPI tables.
    @retval L"EfiACPIMemoryNVS"           Memory is address space reserved for use by the firmware.
    @retval L"EfiMemoryMappedIO"          Memory is used by system firmware to request that a memory-mapped IO region be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
    @retval L"EfiMemoryMappedIOPortSpace" Memory is system memory-mapped IO region that is used to translate memory cycles to IO cycles by the processor.
    @retval L"EfiPalCode"                 Memory is address space reserved by the firmware for code that is part of the processor.
    @retval L"EfiMaxMemoryType"           What is this?
    @retval L"InvalidMemoryType"          Type of memory is unknown.
**/

static EFI_STATUS OpenGOP(
  IN EFI_HANDLE                     imageHandle,
  OUT EFI_GRAPHICS_OUTPUT_PROTOCOL** GOP);

/**
    Initializes and opens Graphics Output Protocol.

    @param  imageHandle           Image Handle.
    @param  GOP                   Protocol of graphics output.

    @retval EFI_SUCCESS           The array of handles was returned in buffer, and the number of handles in buffer was returned in number of buffer.
    @retval EFI_UNSUPPORTED       Handle does not support protocol.
    @retval EFI_INVALID_PARAMETER Number of buffer is NULL, buffer is NULL, Attributes is not legal value or combination of attribute and agent handle is not legal value.
    @retval EFI_ACCESS_DENIED     Attribute is not legal values in some situations.
    @retval EFI_ALREADY_STARTED   Protocol was already opened.
    @retval EFI_NOT_FOUND         No handles match the search.
    @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the matching results.
**/

static CONST CHAR16* GetPixelFormatUnicode(
  IN EFI_GRAPHICS_PIXEL_FORMAT format);

/**
    Gets a format of pixel in Unicode.

    @param  format                                      Format of pixel.

    @retval L"PixelRedGreenBlueReserved8BitPerColor"    A pixel is 32-bits and byte zero represents red, byte one represents green, byte two represents blue, and byte three is reserved. This is the definition for the physical frame buffer. The byte values for the red, green, and blue components represent the color intensity. This color intensity value range from a minimum intensity of 0 to maximum intensity of 255.
    @retval L"PixelBlueGreenRedReserved8BitPerColor"    A pixel is 32-bits and byte zero represents blue, byte one represents green, byte two represents red, and byte three is reserved. This is the definition for the physical frame buffer. The byte values for the red, green, and blue components represent the color intensity. This color intensity value range from a minimum intensity of 0 to maximum intensity of 255.
    @retval L"PixelBitMask"                             The pixel definition of the physical frame buffer.
    @retval L"PixelBltOnly"                             This mode does not support a physical frame buffer.
    @retval L"PixelFormatMax"                           Valid EFI_GRAPHICS_PIXEL_FORMAT enum values are less than this value.
    @retval L"InvalidPixelFormat"                       The pixel has invalid format.
**/

static void GetLoadAddressRange(
  IN VOID* kernelBuffer,
  OUT EFI_PHYSICAL_ADDRESS* start,
  OUT EFI_PHYSICAL_ADDRESS* end);

static void LoadKernelSegment(
  IN VOID* kernelBuffer);

// Miscellaneous Functions

static UINTN AsciiStrLen(
  IN CHAR8* string) {
    UINTN length;

    for (length = 0; *string; ++string, ++length);

    return length;
}

/**
    Counts a length of string.

    @param  string String.

    @retval length Length of the given string.
**/

static UINTN AsciiSPrint(
  OUT CHAR8* buffer,
  IN  UINTN        bufferSize,
  IN  CONST CHAR8* formatString,
  IN  ...) {
    va_list marker;
    UINTN   numberOfPrinted;

    va_start(marker, formatString);
    numberOfPrinted = AsciiVSPrint(buffer, bufferSize, formatString, marker);
    va_end(marker);

    return numberOfPrinted;
}

/**
    Converts format in given string to real value and writes it to given buffer.

    @param  buffer          Buffer.
    @param  bufferSize      Size of buffer.
    @param  formatString    String which has format.

    @retval numberOfPrinted Size of converted string.
**/