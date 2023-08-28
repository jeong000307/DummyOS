#ifndef __HEAP_H__
#define __HEAP_H__

#include "Type.h"
#include "ErrorCode.h"
#include "Memory.h"

typedef struct __HEAP HEAP;

typedef byte* (*CHANGE_PROGRAM_BREAK)(
    IN HEAP* this,
    IN size increment);

struct __HEAP {
    byte* programBreak;
    byte* programBreakLimit;

    CHANGE_PROGRAM_BREAK ChangeProgramBreak;
};

code InitializeHeap(
  MEMORY_MANAGER* memoryManger);

HEAP* GetSystemHeap(void);

static byte* __ChangeProgramBreak(
  HEAP* this, 
  size increment);

void* AllocateMemory(
  size length);

#endif