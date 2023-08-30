#ifndef __TIMER_H__
#define __TIMER_H__

#include "DataStructure.h"
#include "Error.h"
#include "Interrupt.h"
#include "Type.h"

#define MAX_COUNT 0xffffffffu

typedef struct __TIMER_MANAGER TIMER_MANAGER;

typedef void (*CREATE_TIMER)(
  TIMER_MANAGER* this,
  size timeOut,
  int64 value);

struct __TIMER_MANAGER {
    size           tick;

    uint32*        LVTTimer;
    uint32*        initialCount;
    uint32*        currentCount;
    uint32*        divideConfiguration;

    TIMER_QUEUE*   timersQueue;
    MESSAGE_QUEUE* messageQueue;

    CREATE_TIMER   CreateTimer;
};

TIMER_MANAGER* GetTimerManager(void);

code InitializeTimerManager(void);

static void __CreateTimer(
  TIMER_MANAGER* this,
  size           timeOut,
  int64          value);

void TimerOnInterrupt(void);

#endif