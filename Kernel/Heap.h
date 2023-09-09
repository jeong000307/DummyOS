#pragma once

#include "Type.h"
#include "Error.h"
#include "MemoryManager.h"

typedef struct __HEAP HEAP;

typedef addr (*CHANGE_PROGRAM_BREAK)(
  HEAP* this,
  size  increment);

struct __HEAP {
    addr                 programBreak;
    addr                 programBreakLimit;

    CHANGE_PROGRAM_BREAK ChangeProgramBreak;
};

code InitializeHeap(
  MEMORY_MANAGER* memoryManager);

HEAP* GetSystemHeap(void);

static addr __ChangeProgramBreak(
  HEAP* this, 
  size  increment);