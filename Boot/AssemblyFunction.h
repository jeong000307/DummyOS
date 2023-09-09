#pragma once

#include  "efi.h"

extern void AP(void);

extern void Assert(UINT64 value);

extern void Pause(void);

/**
    Halt the processor.
**/