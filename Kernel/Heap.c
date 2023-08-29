#include "Heap.h"

static HEAP systemHeap;

code InitializeHeap(MEMORY_MANAGER* memoryManager) {
    const int   systemHeapFrames = PDP_TABLE_SIZE * PAGE_DIRECTORY_SIZE;
    const frame heapStart = memoryManager->AllocateFrame(memoryManager, systemHeapFrames);

    if (heapStart == (addr)1 << 63) {
        return MEMORY_ERROR;
    }

    systemHeap.programBreak = (heapStart * BYTE_PER_FRAME);
    systemHeap.programBreakLimit = systemHeap.programBreak + systemHeapFrames * BYTE_PER_FRAME;
    systemHeap.ChangeProgramBreak = __ChangeProgramBreak;

    return SUCCESS;
}

HEAP* GetSystemHeap(void) {
    return &systemHeap;
}

static addr __ChangeProgramBreak(HEAP* this, size increment) {
   addr previousBreak;

    if (this->programBreak == 0 or this->programBreak + increment >= this->programBreakLimit) {
        return -1;
    }

    previousBreak = this->programBreak;
    this->programBreak += increment;

    return previousBreak;
}

void* AllocateMemory(size length) {
    HEAP* systemHeap = GetSystemHeap();
    void* startPointer = (void *)systemHeap->ChangeProgramBreak(systemHeap, 0);
    void* endPointer = (void*)systemHeap->ChangeProgramBreak(systemHeap, length);

    if (endPointer == (void*)-1 or startPointer == endPointer) {
        return NULL;
    }
    else {
        return startPointer;
    }
}