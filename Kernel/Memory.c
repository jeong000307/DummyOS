#include "Memory.h"

byte _Alignas(16) systemStack[1024 * 1024] = {0};

static MEMORY_MANAGER memoryManager;

code InitializeMemoryManager(
  const struct MemoryMap* memoryMap) {
    size                     PDPTableIndex;
    size                     pageDirectoryIndex;

    addr                     memoryMapBase = (addr)memoryMap->buffer;
    addr                     availableMemoryEnd = 0;
    addr                     physicalEnd;
    addr                     iterator;

    struct MemoryDescriptor* descriptor;

    memoryManager.AllocateFrame = __AllocateFrame;
    memoryManager.FreeFrame = __FreeFrame;
    memoryManager.MarkAllocatedFrame = __MarkAllocatedFrame;

    memoryManager.PML4Table[0] = (addr)(&memoryManager.PDPTable[0]) | 0x003;

    for (PDPTableIndex = 0; PDPTableIndex < PDP_TABLE_SIZE; ++PDPTableIndex) {
        memoryManager.PDPTable[PDPTableIndex] = (addr)(&memoryManager.pageDirectory[PDPTableIndex]) | 0x003;

        for (pageDirectoryIndex = 0; pageDirectoryIndex < PAGE_DIRECTORY_SIZE; ++pageDirectoryIndex) {
            memoryManager.pageDirectory[PDPTableIndex][pageDirectoryIndex] = PDPTableIndex * GiB(1) + pageDirectoryIndex * MiB(2) | 0x083;
        }
    }

    SetCR3((addr)&memoryManager.PML4Table[0]);

    for (iterator = memoryMapBase; iterator < memoryMapBase + memoryMap->mapSize; iterator += memoryMap->descriptorSize) {
        descriptor = (struct MemoryDescriptor*)iterator;
        physicalEnd = descriptor->physicalStart + descriptor->numberOfPages * EFI_PAGE_SIZE;
        if (availableMemoryEnd < descriptor->physicalStart) {
            memoryManager.MarkAllocatedFrame(&memoryManager, availableMemoryEnd / BYTE_PER_FRAME, (descriptor->physicalStart - availableMemoryEnd) / BYTE_PER_FRAME);
        }

        if (IsUsableMemory(descriptor->type) == true) {
            availableMemoryEnd = physicalEnd;
        } else {
            memoryManager.MarkAllocatedFrame(&memoryManager, descriptor->physicalStart / BYTE_PER_FRAME, descriptor->numberOfPages * EFI_PAGE_SIZE / BYTE_PER_FRAME);
        }
    }

    memoryManager.beginFrame = 1;
    memoryManager.endFrame = availableMemoryEnd / BYTE_PER_FRAME;

    return SUCCESS;
}

MEMORY_MANAGER* GetMemoryManager(void) {
    return &memoryManager;
}

static bool IsUsableMemory(enum MemoryType memoryType) {
    size       index;

    const byte AvailableMemoryTypes[3] = {
        EfiBootServicesCode,
        EfiBootServicesData,
        EfiConventionalMemory
    };

    for (index = 0; index < sizeof(AvailableMemoryTypes) / sizeof(byte); ++index) {
        if (memoryType == AvailableMemoryTypes[index]) {
            return true;
        }
    }

    return false;
}

static frame __AllocateFrame(MEMORY_MANAGER* this, size numberOfFrames) {
    size index = 0;

    while (true) {
        for (index = 0; index < numberOfFrames; ++index) {
            if (this->beginFrame + index >= this->endFrame) {
                return NULL_FRAME;
            }

            if (GetBit(this, this->beginFrame + index)) {
                break;
            }
        }

        if (index == numberOfFrames) {
            this->MarkAllocatedFrame(this, this->beginFrame, numberOfFrames);
            return this->beginFrame;
        }

        this->beginFrame += index + 1;
    }
}

static code __FreeFrame(MEMORY_MANAGER* this, frame startFrame, size numberOfFrames) {
    size index;

    for (index = 0; index < numberOfFrames; ++index) {
        SetBit(this, startFrame + 1, false);
    }

    return SUCCESS;
}

static void __MarkAllocatedFrame(MEMORY_MANAGER* this, frame startFrame, size numberOfFrames) {
    size index;

    for (index = 0; index < numberOfFrames; ++index) {
        SetBit(this, startFrame + index, true);
    }
}

static bool GetBit(MEMORY_MANAGER* memoryManager, frame frame) {
    size lineIndex = frame / (8 * sizeof(frame));
    size bitIndex = frame % (8 * sizeof(frame));

    return (memoryManager->allocationMap[lineIndex] & (1 << bitIndex)) != 0;
}

static void SetBit(MEMORY_MANAGER* memoryManager, frame frame, bool allocated) {
    size lineIndex = frame / (8 * sizeof(frame));
    size bitIndex = frame % (8 * sizeof(frame));

    if (allocated) {
        memoryManager->allocationMap[lineIndex] |= 1 << bitIndex;
    } else {
        memoryManager->allocationMap[lineIndex] &= 1 << bitIndex;
    }
}

code SetMemory(
  void*  destination,
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
    byte* destinationPointer = (byte *)destination;
    byte* sourcePointer = (byte *)source;

    if (length == 0 or destination == source) {
        return SUCCESS;
    }

    if (destination == NULL or source == NULL) {
        return MEMORY_ERROR;
    }

    if (sourcePointer < destinationPointer) {
        while (length > 0) {
            *(destinationPointer + length) = *(sourcePointer + length);

            --length;
        }
    }
    else {
        while (length > 0) {
            *destinationPointer++ = *sourcePointer++;

            --length;
        }
    }

    return SUCCESS;
}