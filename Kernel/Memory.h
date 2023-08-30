#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Type.h"
#include "Heap.h"

void* AllocateMemory(
  size length);

code SetMemory(
  void* destination,
  byte   value,
  size   length);

code CopyMemory(
  void* source,
  void* destination,
  size  length);

#endif