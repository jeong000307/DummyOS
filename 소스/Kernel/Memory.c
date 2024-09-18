#include "Memory.h"

VOID*
DUMMYAPI AllocateMemory(
	IN CONST	UINTN	_length
)   {
	HEAP*   systemHeap = GetSystemHeap();
	VOID*   startPointer = (VOID*)systemHeap->ChangeProgramBreak(systemHeap, 0);
	VOID*   endPointer = (VOID*)systemHeap->ChangeProgramBreak(systemHeap, _length);

	if (endPointer == (VOID*)-1 OR _length == 0) {
		return NULL;
	}
	else {
		return startPointer;
	}
}

STATUS
DUMMYAPI SetMemory(
	OUT			VOID*   _destination,
	IN CONST	BYTE	_value,
	IN CONST	UINTN	_length
)   {
	UINTN   length = _length;
	BYTE*   pointer = _destination;

	if (_length == 0) {
		return SUCCESS;
	}

	if (_destination == NULL) {
		return ERR_DEVICE_ERROR;
	}

	while (length > 0) {
		*pointer++ = _value;

		--length;
	}

	return SUCCESS;
}

STATUS
DUMMYAPI CopyMemory(
	IN CONST	VOID*	_source,
	OUT	CONST	VOID*	_destination,
	IN CONST	UINTN	_length
)	{
	UINTN	length = _length;

	BYTE*	destinationPointer = (BYTE*)_destination;
	BYTE*	sourcePointer = (BYTE*)_source;

	if (_length == 0 OR _destination == _source) {
		return SUCCESS;
	}

	if (_destination == NULL OR _source == NULL) {
		return ERR_DEVICE_ERROR;
	}

	if (sourcePointer > destinationPointer) {
		while (length > 0) {
			*destinationPointer++ = *sourcePointer++;

			--length;
		}
	}
	else {
		while (length > 0) {
			*(destinationPointer + length - 1) = *(sourcePointer + length - 1);

			--length;
		}
	}

	return SUCCESS;
}