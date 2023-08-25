#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned char      byte;
typedef signed char        code;
typedef unsigned long long size;

typedef _Bool              bool;
typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
typedef unsigned char*     va_list;

#define false 0
#define true  1

#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_arg(list, type)    (*(type *)((list += sizeof(type)) - sizeof(type)))
#define va_end(param)         (param = (va_list)0)

#define IN
#define OUT

#define or  ||
#define and &&

#define NULL (void *)0

#endif