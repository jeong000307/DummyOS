#ifndef __HEAP_H__
#define __HEAP_H__

#include "Error.h"
#include "MemoryManager.h"
#include "Type.h"

typedef struct _HEAP HEAP;

struct _HEAP {
	ADDRESS programBreak;
	ADDRESS programBreakLimit;

	ADDRESS (DUMMYAPI* ChangeProgramBreak)(
				OUT			HEAP*   _this,
				IN CONST    UINTN	_increment
			);
};

STATUS 
DUMMYAPI InitializeHeap(
	IN	MEMORY_MANAGER*	_memoryManager
);

HEAP* 
DUMMYAPI GetSystemHeap(
	VOID
);

STATIC ADDRESS 
DUMMYAPI _ChangeProgramBreak(
	OUT			HEAP*   _this, 
	IN CONST	UINTN   _increment
);

#endif