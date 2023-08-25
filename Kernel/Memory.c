#include "Memory.h"

code SetMemory(
  void*  destination,
  byte   value,
  uint64 size
) {
    uint8* pointer = destination;

    if (size == 0) {
        return INVALID_PARAMETER;
    }

    if (destination == NULL) {
        return MEMORY_ERROR;
    }

    while (size > 0) {
        *pointer = value;
        ++pointer;
        --size;
    }

    return SUCCESS;
}

code CopyMemory(
  void*       destination,
  const void* source,
  uint64      size
) {
    byte*       destinationPointer = destination;
    const byte* sourcePointer = source;

    if (size == 0) {
        return INVALID_PARAMETER;
    }

    if (destination == source 
      or GetAbsoluteValue(destinationPointer - sourcePointer) < size) {
        return MEMORY_ERROR;
    }

    while (size) {
        *destinationPointer = *sourcePointer;

        ++destinationPointer;
        ++sourcePointer;
        --size;
    }

    return SUCCESS;
}