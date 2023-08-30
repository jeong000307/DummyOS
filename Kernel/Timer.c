#include "Timer.h"

static TIMER timer;

TIMER* GetTimer(void) {
    return &timer;
}

code InitializeTimer(void) {
    timer.LVTTimer = (uint32 *)0xfee00320;
    timer.initialCount = (uint32 *)0xfee00380;
    timer.currentCount = (uint32 *)0xfee00390;
    timer.divideConfiguration = (uint32 *)0xfee003e0;

    *timer.divideConfiguration = 0b1011;
    *timer.LVTTimer = (0b010 << 16) | TimerInterruptIndex;
    *timer.initialCount = 0x1000000u;

    timer.tick = 0;

    timer.StartTimer = __StartTimer;
    timer.CountTime = __GetTime;
    timer.StopTimer = __StopTimer;

    return SUCCESS;
}

static void __StartTimer(
  TIMER* this) {
    *this->initialCount = MAX_COUNT;
}

static uint32 __GetTime(
  TIMER* this) {
    return MAX_COUNT - *this->currentCount;
}

static void __StopTimer(
  TIMER* this) {
    *this->initialCount = 0;
}

void TimerOnInterrupt(void) {
    MESSAGE_QUEUE* messageQueue = GetMessageQueue();

    ++timer.tick;
    messageQueue->Push(messageQueue, (struct Message) { TimerInterruptIndex });
}