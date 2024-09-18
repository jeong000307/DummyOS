#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Type.h"
#include "Heap.h"

VOID* 
DUMMYAPI AllocateMemory(
	IN CONST	UINTN	_length
);

STATUS
DUMMYAPI SetMemory(
	OUT			VOID*	_destination,
	IN CONST	BYTE	_value,
	IN CONST	UINTN	_length
);

STATUS
DUMMYAPI CopyMemory(
	IN CONST	VOID*	_source,
	OUT	CONST	VOID*	_destination,
	IN CONST	UINTN	_length
);

#endif