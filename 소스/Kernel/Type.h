#ifndef __TYPE_H__
#define __TYPE_H__

#define MAX_INT8    0x7F
#define MAX_INT16   0x7FFF
#define MAX_INT32   0x7FFFFFFF
#define MAX_INT64   0x7FFFFFFFFFFFFFFF
#define MAX_UINT8   0xFF
#define MAX_UINT16  0xFFFF
#define MAX_UINT32  0xFFFFFFFF
#define MAX_UINT64  0xFFFFFFFFFFFFFFFF

#define MIN_INT8    (-MAX_INT8 - 1)
#define MIN_INT16   (-MAX_INT16 - 1)
#define MIN_INT32   (-MAX_INT32 - 1)
#define MIN_INT64   (-MAX_INT64 - 1)

// 한정자

#define CONST		const
#define STATIC		static
#define EXTERN		extern
#define VOID		void

#define IN 
#define OUT 
#define OPTIONAL 

#define DUMMYAPI	__attribute__((ms_abi))
#define NORETURN	__attribute__((noreturn))

// 상수 정의

#define TRUE	((BOOLEAN)1)
#define FALSE	((BOOLEAN)0)

#define NUL		'\0'
#define NULL	((VOID*)0)

#define OR		||
#define AND		&&

#define INT_SIZEOF(type)			((sizeof(type) + sizeof(UINTN) - 1) & ~(sizeof(UINTN) - 1))
#define VA_START(marker, parameter)	(marker = (VA_LIST)((UINTN)&parameter + INT_SIZEOF(parameter)))
#define VA_ARG(marker, type)		(*(type*)((marker += INT_SIZEOF(type)) - INT_SIZEOF(type)))
#define VA_END(marker)				(marker = (VA_LIST)0)
#define VA_COPY(destination, start)	((VOID)((destination) = (start)))

#define KiB(param)          (param * 1024)
#define MiB(param)          KiB(param * 1024)
#define GiB(param)          MiB(param * 1024)

typedef char                BYTE;
typedef unsigned char		BOOLEAN;

typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned int    	UINT32;
typedef unsigned long long	UINT64;

typedef signed char			INT8;
typedef short				INT16;
typedef int 				INT32;
typedef long long			INT64;

typedef UINT64				UINTN;
typedef INT64				INTN;

typedef UINTN				STATUS;
typedef UINTN				ADDRESS;
typedef UINT64				FRAME;
typedef UINT32				BITMAP;

typedef BYTE*				VA_LIST;

/*
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
*/

#endif