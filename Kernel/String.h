#ifndef __STRING_H__
#define __STRING_H__

#include "Type.h"
#include "AssemblyFunction.h"

size GetStringLength(
    const byte* string);

void CopyString(
  const byte* source,
  byte* destination);

int64 CompareString(
  const byte* source1,
  const byte* source2,
  size        length);

void ConvertDecimalToString(byte* string, uint64 number, bool sign);

void ConvertHexadecimalToString(byte* string, uint64 number, bool caps);

#endif