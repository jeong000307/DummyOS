#ifndef __HEAP_H__
#define __HEAP_H__

#include "Type.h"
#include "Error.h"
#include "MemoryManager.h"

typedef struct __HEAP HEAP;

typedef addr (*CHANGE_PROGRAM_BREAK)(
  HEAP* this,
  size increment);

struct __HEAP {
    addr programBreak;
    addr programBreakLimit;

    CHANGE_PROGRAM_BREAK ChangeProgramBreak;
};

code InitializeHeap(
  MEMORY_MANAGER* memoryManger);

HEAP* GetSystemHeap(void);

static addr __ChangeProgramBreak(
  HEAP* this, 
  size increment);

void* AllocateMemory(
  size length);

#endif