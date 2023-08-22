#ifndef __MAIN_H__
#define __MAIN_H__

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

struct MEMORY_MAP {
    UINTN	bufferSize;
    VOID*	buffer;
    UINTN	mapSize;
    UINTN	mapKey;
    UINTN	descriptorSize;
    UINT32	descriptorVersion;
};

EFI_STATUS GetMemoryMap(
    struct MEMORY_MAP* map);

/**
    Gets a memory map.

    @param map	Memory map.

    @retval EFI_SUCCESS				The memory map is gotten.
    @retval EFI_BUFFER_TOO_SMALL	Buffer is smaller than the memory map's size.
**/

EFI_STATUS OpenRootDirectory(
    EFI_HANDLE imageHandle,
    EFI_FILE_PROTOCOL** rootDirectory);

/**
    Opens a root directory of boot device.

    @param	ImageHandle		Image handle.
    @param	rootDirectory	Root directory of boot device.

    @retval EFI_SUCCESS	The root directory of boot device is opened.
**/

EFI_STATUS SaveMemoryMap(
    struct MEMORY_MAP* map,
    EFI_FILE_PROTOCOL* file);

/**
    Saves a memory map.

    @param	map		Memory map.
    @param	file	File to save a memory map.

    @retval EFI_SUCCESS	The file is written correctly.
**/

const CHAR16* GetMemoryTypeUnicode(
    EFI_MEMORY_TYPE type);

/**
    Gets a memory type in Unicode.

    @param	type	Type of memory.

    @retval L"EfiReservedMemoryType"		Memory is not used.
    @retval L"EfiLoaderCode"				Memory is the code portions of a loaded application.
    @retval L"EfiLoaderData"				Memory is the data portions of a loaded application and the default data allocation type used by an application to allocate pool memory.
    @retval L"EfiBootServicesCode"			Memory is the code portions of a loaded boot services driver.
    @retval L"EfiBootServicesData"			Memory is the data portions of a loaded boot services driver, and the default data allocation type used by a boot services driver to allocate pool memory.
    @retval L"EfiRuntimeServicesCode"		Memory is the code portions of a loaded runtime services driver.
    @retval L"EfiRuntimeServicesData"		Memory is the data portions of a loaded runtime services driver and the default data allocation type used by a runtime services driver to allocate pool memory.
    @retval L"EfiConventionalMemory"		Memory is free.
    @retval L"EfiUnusableMemory"			Memory has errors.
    @retval L"EfiACPIReclaimMemory"			Memory has ACPI tables.
    @retval L"EfiACPIMemoryNVS"				Memory is address space reserved for use by the firmware.
    @retval L"EfiMemoryMappedIO"			Memory is used by system firmware to request that a memory-mapped IO region be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
    @retval L"EfiMemoryMappedIOPortSpace"	Memory is system memory-mapped IO region that is used to translate memory cycles to IO cycles by the processor.
    @retval L"EfiPalCode"					Memory is address space reserved by the firmware for code that is part of the processor.
    @retval L"EfiPersistentMemory"			Memory is a memory region that operates as EfiConventionalMemory, however it happens to also support byte-addressable non-volatility.
    @retval L"EfiMaxMemoryType"				What is this?
    @retval L"InvalidMemoryType"			Type of memory is unknown.
**/

#endif
