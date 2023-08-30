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

#define MAX_INT8    0x7F
#define MAX_INT16   0x7FFF
#define MAX_INT32   0x7FFFFFFF
#define MAX_INT64   0x7FFFFFFFFFFFFFFF
#define MAX_UINT8   0xFF
#define MAX_UINT16  0xFFFF
#define MAX_UINT32  0xFFFFFFFF
#define MAX_UINT64  0xFFFFFFFFFFFFFFFF

#define NULL  0

#define false 0
#define true  1

#define or  ||
#define and &&

#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_arg(list, type)    (*(type *)((list += sizeof(type)) - sizeof(type)))
#define va_end(param)         (param = (va_list)0)

#define KiB(param)          (param * 1024)
#define MiB(param)          KiB(param * 1024)
#define GiB(param)          MiB(param * 1024)

#endif