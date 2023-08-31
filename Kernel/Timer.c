#include "Timer.h"

static TIMER_MANAGER timerManager;

TIMER_MANAGER* GetTimerManager(void) {
    return &timerManager;
}

code InitializeTimerManager(void) {
    uint32 count;

    timerManager.LVTTimer = (uint32*)0xfee00320;
    timerManager.initialCount = (uint32*)0xfee00380;
    timerManager.currentCount = (uint32*)0xfee00390;
    timerManager.divideConfiguration = (uint32*)0xfee003e0;

    timerManager.CreateTimer = __CreateTimer;
    timerManager.StartTimer = __StartTimer;
    timerManager.CountTime = __CountTime;
    timerManager.StopTimer = __StopTimer;

    InitializeTimerQueue(timerManager.timersQueue, 256);

    timerManager.CreateTimer(&timerManager, MAX_UINT32, -1);

    *timerManager.divideConfiguration = 0b1011;
    *timerManager.LVTTimer = 0b001 << 16;

    timerManager.StartTimer(&timerManager);
    WaitMilliSeconds(100);
    count = timerManager.CountTime(&timerManager);
    timerManager.StopTimer(&timerManager);

    timerManager.timerFrequency = count * 10;

    *timerManager.divideConfiguration = 0b1011;
    *timerManager.LVTTimer = (0b010 << 16) | TimerInterruptIndex;
    *timerManager.initialCount = timerManager.timerFrequency / 100;
    timerManager.tick = 0;

    return SUCCESS;
}

void __CreateTimer(
  TIMER_MANAGER* this,
  size           timeOut,
  int64          value) {
    this->timersQueue->Push(this->timersQueue, (struct Timer){ timeOut, value });
    
    return ;
}

static void __StartTimer(TIMER_MANAGER* this) {
    *this->initialCount = MAX_COUNT;
}

static uint32 __CountTime(TIMER_MANAGER* this) {
    return MAX_COUNT - *this->currentCount;
}

static void __StopTimer(TIMER_MANAGER* this) {
    *this->initialCount = 0;
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