#ifndef __STRING_H__
#define __STRING_H__

#include "Type.h"
#include "AssemblyFunction.h"

size GetStringLength(
    const byte* string);

void CopyString(byte* destination, const byte* source);

void ConvertDecimalToString(byte* string, int64 number);

void ConvertHexadecimalToString(byte* string, uint64 number);

#endif