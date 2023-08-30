#ifndef __TIMER_H__
#define __TIMER_H__

#include "Interrupt.h"
#include "Type.h"
#include "Error.h"

typedef struct __TIMER_MANAGER TIMER_MANAGER;
typedef struct __TIMER TIMER;

typedef void (*CREATE_TIMER)(
  TIMER_MANAGER* this);

struct __TIMER_MANAGER {
    TIMER*       timers;

    CREATE_TIMER CreateTimer;
};

typedef void (*START_TIMER)(
  TIMER* this);

typedef uint32 (*GET_TIME)(
  TIMER* this);

typedef void (*STOP_TIMER)(
  TIMER* this);

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

code InitializeTimer(void);

static void __CreateTimer(
  TIMER_MANAGER* this);

static void __StartTimer(
  TIMER* this);

static uint32 __GetTime(
  TIMER* this);

static void __StopTimer(
  TIMER* this);

//void TimerOnInterrupt(void);

#endif