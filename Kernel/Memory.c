#include "Memory.h"

void* AllocateMemory(
  size length) {
    HEAP* systemHeap = GetSystemHeap();
    void* startPointer = (void*)systemHeap->ChangeProgramBreak(systemHeap, 0);
    void* endPointer = (void*)systemHeap->ChangeProgramBreak(systemHeap, length);

    if (endPointer == (void*)-1 or length == 0) {
        return NULL;
    }
    else {
        return startPointer;
    }
}

code SetMemory(
  void* destination,
  byte   value,
  size   length) {
    byte* pointer = destination;

    if (length == 0) {
        return SUCCESS;
    }

    if (destination == NULL) {
        return MEMORY_ERROR;
    }

    while (length > 0) {
        *pointer++ = value;

        --length;
    }

    return SUCCESS;
}

code CopyMemory(
  void* source,
  void* destination,
  size  length) {
    byte* destinationPointer = (byte*)destination;
    byte* sourcePointer = (byte*)source;

    if (length == 0 or destination == source) {
        return SUCCESS;
    }

    if (destination == NULL or source == NULL) {
        return MEMORY_ERROR;
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