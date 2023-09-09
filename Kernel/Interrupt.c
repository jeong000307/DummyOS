#include "Interrupt.h"

static struct InterruptDescriptor IDT[256];

static MESSAGE_QUEUE messageQueue;

MESSAGE_QUEUE* GetMessageQueue(void) {
    return &messageQueue;
}

code InitializeInterrupt(void) {
    InitializeMessageQueue(&messageQueue, 256);

    SetIDTEntry(&IDT[TimerInterruptIndex], MakeIDTAttribute(InterruptGate, 0, true, 0), (addr)TimerInterruptHandler, 1 << 3);
    LoadIDT(sizeof(IDT) - 1, (addr)&IDT[0]);

    return SUCCESS;
}