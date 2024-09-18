#include "Math.h"

UINT64
DUMMYAPI GetAbsoluteValue(
	IN CONST	INT64	value
)	{
	return value > 0? value: -value;
}