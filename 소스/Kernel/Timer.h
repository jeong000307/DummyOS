#ifndef __TIMER_H__
#define __TIMER_H__

#include "ACPI.h"
#include "Error.h"
#include "Interrupt.h"
#include "Message.h"
#include "Type.h"

#define MAX_COUNT 0xFFFFFFFFu

typedef struct _TIMER_QUEUE TIMER_QUEUE;
typedef struct _TIMER_MANAGER TIMER_MANAGER;

typedef enum {
	taskTimerPeriod = 2
} TIMER_PERIOD;

typedef enum {
	taskTimerValue = MIN_INT32
} TIMER_VALUE;

typedef struct {
	UINTN   timeOut;
	INTN    value;
} TIMER;

struct _TIMER_QUEUE {
	UINTN	count;
	UINTN	capacity;

	TIMER* timers;

	STATUS	(DUMMYAPI* Push) (
				OUT			TIMER_QUEUE*	_this,
				IN CONST	TIMER			_value
			);
	TIMER	(DUMMYAPI* Pop) (
				OUT	TIMER_QUEUE*	_this
			);
	TIMER*	(DUMMYAPI* Top) (
				IN CONST	TIMER_QUEUE*	_this
			);
};

struct _TIMER_MANAGER {
	UINT32			tick;
	UINT32			timerFrequency;

	UINT32*			LVTTimer;
	UINT32*			initialCount;
	UINT32*			currentCount;
	UINT32*			divideConfiguration;

	TIMER_QUEUE*	timersQueue;
	MESSAGE_QUEUE*	messageQueue;

	VOID			(DUMMYAPI *CreateTimer)(
						OUT			TIMER_MANAGER*	_this,
						IN CONST	UINTN			_timeOut,
						IN CONST	INTN 			_value
					);
	VOID			(DUMMYAPI *StartTimer)(
						OUT	TIMER_MANAGER*	_this
					);
	UINT32			(DUMMYAPI *CountTime)(
						IN CONST	TIMER_MANAGER* _this
					);
	VOID			(DUMMYAPI *StopTimer)(
						OUT	TIMER_MANAGER*	_this
					);
};

VOID
DUMMYAPI InitializeTimerQueue(
	OUT			TIMER_QUEUE*	_this,
	IN CONST	UINTN           _capacity
);

STATIC STATUS
DUMMYAPI _TimerQueuePush(
	OUT			TIMER_QUEUE*	_this,
	IN CONST	TIMER			_value
);

STATIC TIMER
DUMMYAPI _TimerQueuePop(
	OUT	TIMER_QUEUE* _this
);

STATIC TIMER*
DUMMYAPI _TimerQueueTop(
	IN CONST	TIMER_QUEUE* _this
);

TIMER_MANAGER* 
DUMMYAPI GetTimerManager(
	VOID
);

STATUS 
DUMMYAPI InitializeTimerManager(
	VOID
);

STATIC VOID 
DUMMYAPI _CreateTimer(
	OUT			TIMER_MANAGER*	_this,
	IN CONST	UINTN			_timeOut,
	IN CONST	INTN 			_value
);

STATIC VOID 
DUMMYAPI _StartTimer(
	OUT	TIMER_MANAGER* _this
);

STATIC UINT32 
DUMMYAPI _CountTime(
	IN CONST	TIMER_MANAGER*	_this
);

STATIC VOID 
DUMMYAPI _StopTimer(
	OUT	TIMER_MANAGER* _this
);

BOOLEAN 
DUMMYAPI TimerOnInterrupt(
	VOID
);

VOID 
DUMMYAPI WaitMilliSeconds(
	IN CONST	UINT32	_milliSeconds
);

#endif