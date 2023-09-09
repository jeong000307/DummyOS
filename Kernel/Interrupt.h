#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "AssemblyFunction.h"
#include "Descriptor.h"
#include "Error.h"
#include "Type.h"

enum InterruptVector {
    TimerInterruptIndex = 0x41
};

MESSAGE_QUEUE* GetMessageQueue(void);

code InitializeInterrupt(void);

#endif