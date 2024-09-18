#ifndef __STRING_H__
#define __STRING_H__

#include "AssemblyFunction.h"
#include "Type.h"

UINTN 
DUMMYAPI GetStringLength(
	IN CONST	BYTE*	_string
);

VOID 
DUMMYAPI CopyString(
	IN CONST	BYTE*	_source,
	OUT			BYTE*	_destination
);

INT64 
DUMMYAPI CompareString(
	IN CONST	BYTE*	_source1,
	IN CONST	BYTE*	_source2,
	IN CONST	UINTN	_length
);

VOID 
DUMMYAPI ConvertDecimalToString(
	OUT			BYTE*	_string, 
	IN CONST	UINT64	_number, 
	IN CONST	BOOLEAN	_sign
);

VOID 
DUMMYAPI ConvertHexadecimalToString(
	OUT			BYTE*	_string, 
	IN CONST	UINT64	_number, 
	IN CONST	BOOLEAN _caps
);

#endif