#include "Timer.h"

static TIMER_MANAGER timerManager;

TIMER_MANAGER* GetTimerManager(void) {
    return &timerManager;
}

code InitializeTimerManager(void) {
    timerManager.LVTTimer = (uint32*)0xfee00320;
    timerManager.initialCount = (uint32*)0xfee00380;
    timerManager.currentCount = (uint32*)0xfee00390;
    timerManager.divideConfiguration = (uint32*)0xfee003e0;

    *timerManager.divideConfiguration = 0b1011;
    *timerManager.LVTTimer = (0b010 << 16) | TimerInterruptIndex;
    *timerManager.initialCount = 0x1000000u;
    timerManager.tick = 0;

    InitializeTimerQueue(timerManager.timersQueue, 256);

    timerManager.CreateTimer = __CreateTimer;

    timerManager.CreateTimer(&timerManager, MAX_UINT64, -1);

    return SUCCESS;
}

void __CreateTimer(
  TIMER_MANAGER* this,
  size           timeOut,
  int64          value) {
    this->timersQueue->Push(this->timersQueue, (struct Timer){ timeOut, value });
    
    return ;
}

void TimerOnInterrupt(void) {
    struct Message message;

    struct Timer prioritizedTimer;
    MESSAGE_QUEUE* messageQueue = GetMessageQueue();

    ++timerManager.tick;

    while (true) {
        prioritizedTimer = *timerManager.timersQueue->Top(timerManager.timersQueue);

        if (prioritizedTimer.timeOut == 0 or prioritizedTimer.timeOut > timerManager.tick) {
            break;
        }

        prioritizedTimer = timerManager.timersQueue->Pop(timerManager.timersQueue);

        message.type = TimerInterruptIndex;
        message.argument.timer.timeOut = prioritizedTimer.timeOut;
        message.argument.timer.value = prioritizedTimer.value;

        messageQueue->Push(messageQueue, message);
        
    }
}

//TIMER* GetTimer(void) {
//    return &timer;
//}
//
//code InitializeTimer(void) {
//
//
//
//    timer.tick = 0;
//
//    timer.StartTimer = __StartTimer;
//    timer.CountTime = __GetTime;
//    timer.StopTimer = __StopTimer;
//
//    return SUCCESS;
//}
//
//static void __StartTimer(
//    TIMER* this) {
//    *this->initialCount = MAX_COUNT;
//}
//
//static uint32 __GetTime(
//    TIMER* this) {
//    return MAX_COUNT - *this->currentCount;
//}
//
//static void __StopTimer(
//    TIMER* this) {
//    *this->initialCount = 0;
//}