#ifndef __TIMER_H__
#define __TIMER_H__

#include "Interrupt.h"
#include "Type.h"
#include "ErrorCode.h"

typedef struct __TIMER TIMER;

typedef void (*START_TIMER)(TIMER* this);
typedef uint32 (*COUNT_TIME)(TIMER* this);
typedef void (*STOP_TIMER)(TIMER* this);

struct __TIMER {
    volatile uint32* LVTTimer;
    volatile uint32* initialCount;
    volatile uint32* currentCount;
    volatile uint32* divideConfiguration;

    volatile uint32 tick;

    START_TIMER StartTimer;
    COUNT_TIME CountTime;
    STOP_TIMER StopTimer;
};

TIMER* GetTimer(void);

code InitializeTimer(void);

void __StartTimer(TIMER* this);

uint32 __CountTime(TIMER* this);

void __StopTimer(TIMER* this);

#endif