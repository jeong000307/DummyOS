#ifndef __TIMER_H__
#define __TIMER_H__

#include "Interrupt.h"
#include "Type.h"
#include "ErrorCode.h"

typedef struct __TIMER TIMER;

typedef void (*START_TIMER)(
  IN TIMER* this);

typedef uint32 (*GET_TIME)(
  IN TIMER* this);

typedef void (*STOP_TIMER)(
  IN TIMER* this);

struct __TIMER {
    volatile uint32* LVTTimer;
    volatile uint32* initialCount;
    volatile uint32* currentCount;
    volatile uint32* divideConfiguration;

    volatile uint32 tick;

    START_TIMER StartTimer;
    GET_TIME    CountTime;
    STOP_TIMER  StopTimer;
};

TIMER* GetTimer(void);

code InitializeTimer(void);

static void __StartTimer(
  IN TIMER* this);

static uint32 __GetTime(
  IN TIMER* this);

static void __StopTimer(
  IN TIMER* this);

void TimerOnInterrupt(void);

#endif