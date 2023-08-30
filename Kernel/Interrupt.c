#include "Interrupt.h"

static struct InterruptDescriptor IDT[256];
static MESSAGE_QUEUE messageQueue;

MESSAGE_QUEUE* GetMessageQueue(void) {
    return &messageQueue;
}

void InitializeMessageQueue(
  MESSAGE_QUEUE* meesageQueue) {
    meesageQueue->count = 0;
    meesageQueue->capacity = 256;

    meesageQueue->Push = __MessageQueuePush;
    meesageQueue->Pop = __MessageQueuePop;

    meesageQueue->front = 0;
    meesageQueue->end = 0;

    return;
}

static code __MessageQueuePush(
  MESSAGE_QUEUE* messageQueue,
  struct Message value) {
    if (messageQueue->count >= messageQueue->capacity) {
        return QUEUE_ERROR;
    }

    messageQueue->messages[messageQueue->end++] = value;
    ++messageQueue->count;

    if (messageQueue->end >= messageQueue->capacity) {
        messageQueue->end = 0;
    }

    return SUCCESS;
}

static struct Message __MessageQueuePop(
  MESSAGE_QUEUE* messageQueue) {
  struct Message value;

    if (messageQueue->count <= 0) {
        return;
    }

    value = messageQueue->messages[messageQueue->front++];
    --messageQueue->count;

    if (messageQueue->front >= messageQueue->capacity) {
        messageQueue->front = 0;
    }

    return value;
}

code InitializeInterrupt(
  MESSAGE_QUEUE* messageQueue) {
    InitializeMessageQueue(messageQueue);

    SetIDTEntry(&IDT[TimerInterruptIndex], MakeIDTAttribute(InterruptGate, 0, true, 0), (addr)TimerInterruptHandler, 1 << 3);
    LoadIDT(sizeof(IDT) - 1, &IDT[0]);

    return SUCCESS;
}

union InterruptDescriptorAttribute MakeIDTAttribute(
  enum DescriptorType type,
  byte descriptorPrivilegeLevel,
  bool present,
  byte interruptStackTable) {
    union InterruptDescriptorAttribute attribute;

    attribute.data = 0;

    attribute.bits.interruptStackTable = interruptStackTable;
    attribute.bits.type = type;
    attribute.bits.descriptorPrivilegeLevel = descriptorPrivilegeLevel;
    attribute.bits.present = present;

    return attribute;
}

void SetIDTEntry(
  struct InterruptDescriptor* descriptor,
  union  InterruptDescriptorAttribute attribute,
  uint64 offset,
  uint16 segmentSelector) {
    descriptor->attribute = attribute;
    descriptor->offsetLow = offset & 0xffffu;
    descriptor->offsetMiddle = (offset >> 16) & 0xffffu;
    descriptor->offsetHigh = offset >> 32;
    descriptor->segmentSelector = segmentSelector;
}