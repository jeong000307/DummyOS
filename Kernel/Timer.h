#pragma once

#include "ACPI/ACPI.h"
#include "Error.h"
#include "Interrupt.h"
#include "Type.h"

#define MAX_COUNT 0xffffffffu

enum TimerPeriod {
    taskTimerPeriod = 2
};

enum TimerValue {
    taskTimerValue = MIN_INT32
};

typedef struct __TIMER_MANAGER TIMER_MANAGER;

typedef void (*CREATE_TIMER)(
  TIMER_MANAGER* this,
  size timeOut,
  int64 value);

typedef void (*START_TIMER)(
  TIMER_MANAGER* this);
typedef uint32 (*COUNT_TIME)(
  TIMER_MANAGER* this);
typedef void (*STOP_TIMER)(
  TIMER_MANAGER* this);

struct __TIMER_MANAGER {
    uint32           tick;
    uint32           timerFrequency;

    uint32*        LVTTimer;
    uint32*        initialCount;
    uint32*        currentCount;
    uint32*        divideConfiguration;

    TIMER_QUEUE*   timersQueue;
    MESSAGE_QUEUE* messageQueue;

    CREATE_TIMER   CreateTimer;
    START_TIMER    StartTimer;
    COUNT_TIME     CountTime;
    STOP_TIMER     StopTimer;
};

TIMER_MANAGER* GetTimerManager(void);

code InitializeTimerManager(void);

static void __CreateTimer(
  TIMER_MANAGER* this,
  size           timeOut,
  int64          value);

static void __StartTimer(
  TIMER_MANAGER* this);

static uint32 __CountTime(
  TIMER_MANAGER* this);

static void __StopTimer(
  TIMER_MANAGER* this);

bool TimerOnInterrupt(void);

void WaitMilliSeconds(
  const uint32 milliSeconds);