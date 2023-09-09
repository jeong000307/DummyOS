#include "Type.h"

void InitializeMessageQueue(
  MESSAGE_QUEUE* meesageQueue,
  size           capacity) {
    meesageQueue->count = 0;
    meesageQueue->capacity = capacity;

    meesageQueue->messages = AllocateMemory(meesageQueue->capacity * sizeof(struct Message));

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
        Assert(MESSAGE_QUEUE_EMPTY);
    }

    value = messageQueue->messages[messageQueue->front++];
    --messageQueue->count;

    if (messageQueue->front >= messageQueue->capacity) {
        messageQueue->front = 0;
    }

    return value;
}

void InitializeTimerQueue(
  TIMER_QUEUE* this,
  size         capacity) {
    this->count = 0;
    this->capacity = capacity;

    this->timers = AllocateMemory((this->capacity + 1) * sizeof(struct Timer));

    this->Push = __TimerQueuePush;
    this->Pop = __TimerQueuePop;
    this->Top = __TimerQueueTop;

    return;
}

static code __TimerQueuePush(
  TIMER_QUEUE*  this,
  struct Timer value) {
    size index;
    size parentIndex;

    if (this->count >= this->capacity) {
        return TIMER_ERROR;
    }

    index = ++this->count;
    parentIndex = index / 2;

    while (index > 1 and value.timeOut < (this->timers[parentIndex]).timeOut) {
        this->timers[index] = this->timers[parentIndex];

        index /= 2;
        parentIndex = index / 2;
    }

    this->timers[index] = value;

    return SUCCESS;
}

static struct Timer __TimerQueuePop(
  TIMER_QUEUE* this) {
    size index = 1;
    size left;
    size right;
    size small;

    struct Timer buffer;
    struct Timer value;

    if (this->count <= 0) {
        Assert(TIMER_QUEUE_EMPTY);
    }

    value = this->timers[index];

    this->timers[index] = this->timers[this->count--];

    while (index < this->count) {
        left = 2 * index;
        right = 2 * index + 1;
        small = left;

        if (right < this->count and this->timers[left].timeOut > this->timers[right].timeOut) {
            small = right;
        }

        if (this->timers[index].timeOut > this->timers[small].timeOut) {
            buffer = this->timers[index];
            this->timers[index] = this->timers[small];
            this->timers[small] = buffer;
        }

        index = small;
    }

    return value;
}

static struct Timer* __TimerQueueTop(
  TIMER_QUEUE* timerQueue) {
    return &(timerQueue->timers[1]);
}