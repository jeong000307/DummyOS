#include "MemoryManager.h"

__attribute__((aligned(16))) BYTE systemStack[1024 * 1024];

STATIC MEMORY_MANAGER memoryManager;

STATUS
DUMMYAPI InitializeMemoryManager(
	IN CONST	MEMORY_INFORMATION* _memoryInformation
)	{
	UINTN				PDPTableIndex;
	UINTN				pageDirectoryIndex;

	ADDRESS             availableMemoryEnd = 0;
	ADDRESS             physicalEnd;
	ADDRESS             iterator;

	MEMORY_DESCRIPTOR*  descriptor;

	memoryManager.AllocateFrame = _AllocateFrame;
	memoryManager.FreeFrame = _FreeFrame;
	memoryManager.MarkAllocatedFrame = _MarkAllocatedFrame;

	memoryManager.PML4Table[0] = (ADDRESS)(&memoryManager.PDPTable[0]) | 0x003;

	for (PDPTableIndex = 0; PDPTableIndex < PDP_TABLE_SIZE; ++PDPTableIndex) {
		memoryManager.PDPTable[PDPTableIndex] = (ADDRESS)(&memoryManager.pageDirectory[PDPTableIndex]) | 0x003;

		for (pageDirectoryIndex = 0; pageDirectoryIndex < PAGE_DIRECTORY_SIZE; ++pageDirectoryIndex) {
			memoryManager.pageDirectory[PDPTableIndex][pageDirectoryIndex] = PDPTableIndex * GiB(1) + pageDirectoryIndex * MiB(2) | 0x083;
		}
	}

	SetCR3((ADDRESS)&memoryManager.PML4Table[0]);

	for (iterator = (ADDRESS)_memoryInformation->memoryMap; iterator < (ADDRESS)_memoryInformation->memoryMap + _memoryInformation->mapSize; iterator += _memoryInformation->descriptorSize) {
		descriptor = (MEMORY_DESCRIPTOR*)iterator;
		physicalEnd = descriptor->physicalStart + descriptor->numberOfPages * EFI_PAGE_SIZE;

		if (availableMemoryEnd < descriptor->physicalStart) {
			memoryManager.MarkAllocatedFrame(&memoryManager, availableMemoryEnd / BYTES_PER_FRAME, (descriptor->physicalStart - availableMemoryEnd) / BYTES_PER_FRAME);
		}

		if (IsUsableMemory(descriptor->type) == TRUE) {
			availableMemoryEnd = physicalEnd;
		} else {
			memoryManager.MarkAllocatedFrame(&memoryManager, descriptor->physicalStart / BYTES_PER_FRAME, descriptor->numberOfPages * EFI_PAGE_SIZE / BYTES_PER_FRAME);
		}
	}

	memoryManager.beginFrame = 1;
	memoryManager.endFrame = availableMemoryEnd / BYTES_PER_FRAME;

	return SUCCESS;
}

MEMORY_MANAGER*
DUMMYAPI GetMemoryManager(
	VOID
)	{
	return &memoryManager;
}

STATIC BOOLEAN
DUMMYAPI IsUsableMemory(
	IN CONST	UINT32	_memoryType
) {
	return _memoryType == efiBootServicesCode OR _memoryType == efiBootServicesData OR _memoryType == efiConventionalMemory;
}

STATIC FRAME
DUMMYAPI _AllocateFrame(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	UINTN			_numberOfFrames
)	{
	UINTN	index = 0;

	for(;;) {
		for (index = 0; index < _numberOfFrames; ++index) {
			if (_this->beginFrame + index >= _this->endFrame) {
				return NULL_FRAME;
			}

			if (GetBit(_this, _this->beginFrame + index)) {
				break;
			}
		}

		if (index == _numberOfFrames) {
			_this->MarkAllocatedFrame(_this, _this->beginFrame, _numberOfFrames);
			return _this->beginFrame;
		}

		_this->beginFrame += index + 1;
	}
}

STATIC STATUS
DUMMYAPI _FreeFrame(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	FRAME			_startFrame,
	IN CONST	UINTN			_numberOfFrames
)	{
	UINTN	index;

	for (index = 0; index < _numberOfFrames; ++index) {
		SetBit(_this, _startFrame + 1, FALSE);
	}

	return SUCCESS;
}

STATIC VOID
DUMMYAPI _MarkAllocatedFrame(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	FRAME			_startFrame,
	IN CONST	UINTN			_numberOfFrames
) {
	UINTN	index;

	for (index = 0; index < _numberOfFrames; ++index) {
		SetBit(_this, _startFrame + index, TRUE);
	}
}

STATIC BOOLEAN
DUMMYAPI GetBit(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	FRAME			_frame
)	{
	UINTN	lineIndex = _frame / BITS_PER_LINE;
	UINTN	bitIndex = _frame % BITS_PER_LINE;

	return (_this->allocationMap[lineIndex] & ((BITMAP)1 << bitIndex)) != 0;
}

STATIC VOID
DUMMYAPI SetBit(
	OUT			MEMORY_MANAGER* _this,
	IN CONST	FRAME			_frame,
	IN CONST	BOOLEAN			_allocated
)	{
	UINTN	lineIndex = _frame / BITS_PER_LINE;
	UINTN	bitIndex = _frame % BITS_PER_LINE;

	if (_allocated == TRUE) {
		_this->allocationMap[lineIndex] |= (BITMAP)1 << bitIndex;
	} else {
		_this->allocationMap[lineIndex] &= ~((BITMAP)1 << bitIndex);
	}
}