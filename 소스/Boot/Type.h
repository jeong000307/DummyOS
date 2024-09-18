#ifndef __TYPE_H__
#define __TYPE_H__

// 매크로

#define INT_SIZEOF(type)			((sizeof(type) + sizeof(UINTN) - 1) & ~(sizeof(UINTN) - 1))
#define VA_START(marker, parameter)	(marker = (VA_LIST)((UINTN) & (parameter) + INT_SIZEOF(parameter)))
#define VA_ARG(marker, type)		(*(type*)((marker += INT_SIZEOF(type)) - INT_SIZEOF(type)))
#define VA_END(marker)				(marker = (VA_LIST)0)
#define VA_COPY(destination, start)	((VOID)((destination) = (start)))

// 한정자

#define CONST		const
#define STATIC		static
#define EXTERN		extern
#define VOID		void

#define IN 
#define OUT 
#define OPTIONAL 

#define EFIAPI		__attribute__((ms_abi))
#define NORETURN	__attribute__((noreturn))

// 상수 정의

#define TRUE	((BOOL)1)
#define FALSE	((BOOL)0)

#define NUL		'\0'
#define NULL	((VOID*)0)

#define OR		||
#define AND		&&

// 주요 자료형 재지정

typedef char				BYTE;
typedef unsigned char		BOOL;

typedef unsigned char		CHAR8;
typedef unsigned short		CHAR16;

typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned int		UINT32;
typedef unsigned long long	UINT64;

typedef char				INT8;
typedef short				INT16;
typedef int					INT32;
typedef long long			INT64;

typedef UINT64				UINTN;
typedef INT64				INTN;

// EFI 자료형 지정

typedef UINTN	EFI_STATUS;

typedef UINT64	EFI_PHYSICAL_ADDRESS;
typedef UINT64	EFI_VIRTUAL_ADDRESS;

typedef struct {
	UINT32	Data1;
	UINT16	Data2;
	UINT16	Data3;
	UINT8	Data4[8];
} EFI_GUID;

typedef VOID*	EFI_HANDLE;
typedef VOID*	EFI_EVENT;

// 가변 인자 정의

typedef UINT8* VA_LIST;

#endif