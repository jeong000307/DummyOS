#include "Heap.h"

STATIC  HEAP    systemHeap;

STATUS
DUMMYAPI InitializeHeap(
	IN	MEMORY_MANAGER* _memoryManager
)   {
	CONST   INT64   systemHeapFrames = PDP_TABLE_SIZE * PAGE_DIRECTORY_SIZE;
	CONST   FRAME   heapStart = _memoryManager->AllocateFrame(_memoryManager, systemHeapFrames);

	if (heapStart == (ADDRESS)1 << 63) {
		return ERR_DEVICE_ERROR;
	}

	systemHeap.programBreak = (heapStart * BYTES_PER_FRAME);
	systemHeap.programBreakLimit = systemHeap.programBreak + systemHeapFrames * BYTES_PER_FRAME;
	systemHeap.ChangeProgramBreak = _ChangeProgramBreak;

	return SUCCESS;
}

HEAP*
DUMMYAPI GetSystemHeap(
	VOID
)   {
	return &systemHeap;
}

STATIC ADDRESS
DUMMYAPI _ChangeProgramBreak(
	OUT			HEAP*   _this,
	IN CONST	UINTN   _increment
)   {
	ADDRESS previousBreak;

	if (_this->programBreak == 0 OR _this->programBreak + _increment >= _this->programBreakLimit) {
		Inspect(ERR_DEVICE_ERROR);
	}

	previousBreak = _this->programBreak;
	_this->programBreak += _increment;

	return previousBreak;
}