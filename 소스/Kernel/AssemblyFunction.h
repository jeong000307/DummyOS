#ifndef __ASSEMBLY_FUNCTION_H__
#define __ASSEMBLY_FUNCTION_H__

#include "SystemConfiguration.h"
#include "Type.h"

EXTERN VOID 
DUMMYAPI Start(
	IN CONST	SYSTEM_CONFIGURATION*	_systemInformation
);

EXTERN VOID 
DUMMYAPI Inspect(
	IN CONST	UINT64	_value
);

EXTERN VOID 
DUMMYAPI Halt(
	VOID
);

EXTERN VOID 
DUMMYAPI IOOut32(
	IN CONST	UINT16	_address, 
	IN CONST	UINT32	_data
);

EXTERN UINT32
DUMMYAPI IOIn32(
	IN CONST	UINT16	_address
);

EXTERN VOID
DUMMYAPI LoadGDT(
	IN CONST	UINT16	_limit, 
	IN CONST	UINT64	_offset
);

EXTERN VOID
DUMMYAPI LoadIDT(
	IN CONST	UINT16	_limit,
	IN CONST	UINT64	_offset
);

EXTERN VOID
DUMMYAPI InitializeSegmentRegister(
	IN CONST	UINT16	_csValue,
	IN CONST	UINT16	_ssValue,
	IN CONST	UINT16	_anotherValue
);

EXTERN UINT16
DUMMYAPI GetCS(
	VOID
);

EXTERN UINT16
DUMMYAPI GetSS(
	VOID
);

EXTERN UINT64
DUMMYAPI GetCR3(
	VOID
);

EXTERN VOID
DUMMYAPI SetCR3(
	IN CONST	UINT64	_value
);

EXTERN VOID
DUMMYAPI SwitchContext(
	IN CONST	VOID*	_nextContext,
	IN CONST	VOID*	_currentContext
);

EXTERN VOID
DUMMYAPI EnableApplicationProcessor(
	VOID
);

EXTERN VOID
DUMMYAPI SetInterruptFlag(
	IN CONST	BOOLEAN	_flag
);

EXTERN VOID
DUMMYAPI TimerInterruptHandler(
	VOID
);

#endif