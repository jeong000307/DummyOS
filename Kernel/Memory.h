#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Type.h"
#include "Math.h"
#include "ErrorCode.h"

code SetMemory(
  void*  destination,
  byte   value,
  uint64 size
);

code CopyMemory(
  void*        destination,
  const void*  source,
  uint64       size
);

#endif