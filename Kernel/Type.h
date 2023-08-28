#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned __int64 addr;
typedef signed __int8    byte;
typedef signed __int8    code;
typedef unsigned __int64 size;
typedef unsigned __int64 frame;
typedef unsigned __int32 bitmap;

typedef _Bool            bool;
typedef signed __int8    int8;
typedef __int16          int16;
typedef __int32          int32;
typedef __int64          int64;
typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned __int8* va_list;

#define MAX_INT8    (int8)((1 << 7) - 1)
#define MAX_INT16   (int16)((1 << 15) - 1)
#define MAX_INT32   (int32)((1 << 31) - 1)
#define MAX_INT64   (int64)((1 << 63) - 1)
#define MAX_UINT8   (uint8)((1 << 8) - 1)
#define MAX_UINT16  (uint16)((1 << 16) - 1)
#define MAX_UINT32  (uint32)((1 << 32) - 1)
#define MAX_UINT64  (uint64)((1 << 64) - 1)

#define NULL  0

#define false 0
#define true  1

#define or  ||
#define and &&

#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_arg(list, type)    (*(type *)((list += sizeof(type)) - sizeof(type)))
#define va_end(param)         (param = (va_list)0)

#define PCI_CONFIG_ADDRESS 0x0cf8
#define PCI_CONFIG_DATA 0x0cfc
#define PCI_DEVICE_MAX 32
#define PCI_FUNCTION_MAX 8

#define EFI_PAGE_SIZE       4096
#define PDP_TABLE_SIZE      64
#define PAGE_DIRECTORY_SIZE 512
#define KiB(param)          (param * 1024)
#define MiB(param)          KiB(param * 1024)
#define GiB(param)          MiB(param * 1024)
#define MAX_PHYSICAL_MEMORY GiB(PDP_TABLE_SIZE)
#define BYTE_PER_FRAME      KiB(4)
#define NULL_FRAME          MAX_UINT64

#define MAX_COUNT 0xffffffffu

#endif