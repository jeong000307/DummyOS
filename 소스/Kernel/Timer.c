#include "Timer.h"

STATIC TIMER_MANAGER timerManager;

VOID
DUMMYAPI InitializeTimerQueue(
	OUT			TIMER_QUEUE*	_this,
	IN CONST	UINTN           _capacity
)	{
	_this->count = 0;
	_this->capacity = _capacity;

	_this->timers = AllocateMemory((_this->capacity + 1) * sizeof(TIMER));

	_this->Push = _TimerQueuePush;
	_this->Pop = _TimerQueuePop;
	_this->Top = _TimerQueueTop;

	return;
}

STATIC STATUS
DUMMYAPI _TimerQueuePush(
	OUT			TIMER_QUEUE*	_this,
	IN CONST	TIMER			_value
)	{
	UINTN   index;
	UINTN   parentIndex;

	if (_this->count >= _this->capacity) {
		return ERR_OUT_OF_RESOURCES;
	}

	index = ++_this->count;
	parentIndex = index / 2;

	while (index > 1 AND _value.timeOut < (_this->timers[parentIndex]).timeOut) {
		_this->timers[index] = _this->timers[parentIndex];

		index /= 2;
		parentIndex = index / 2;
	}

	_this->timers[index] = _value;

	return SUCCESS;
}

STATIC TIMER
DUMMYAPI _TimerQueuePop(
	OUT	TIMER_QUEUE*	_this
)	{
	UINTN   index = 1;
	UINTN   left;
	UINTN   right;
	UINTN   small;

	TIMER   buffer;
	TIMER   value;

	if (_this->count <= 0) {
		Inspect(ERR_OUT_OF_RESOURCES);
	}

	value = _this->timers[index];

	_this->timers[index] = _this->timers[_this->count--];

	while (index < _this->count) {
		left = 2 * index;
		right = 2 * index + 1;
		small = left;

		if (right < _this->count AND _this->timers[left].timeOut > _this->timers[right].timeOut) {
			small = right;
		}

		if (_this->timers[index].timeOut > _this->timers[small].timeOut) {
			buffer = _this->timers[index];
			_this->timers[index] = _this->timers[small];
			_this->timers[small] = buffer;
		}

		index = small;
	}

	return value;
}

STATIC TIMER*
DUMMYAPI _TimerQueueTop(
	IN CONST	TIMER_QUEUE*	_this
)	{
	return &(_this->timers[1]);
}

TIMER_MANAGER*
DUMMYAPI GetTimerManager(
	VOID
)	{
	return &timerManager;
}

STATUS
DUMMYAPI InitializeTimerManager(
	VOID
)   {
	UINT32 count;

	timerManager.LVTTimer = (UINT32*)(ADDRESS)0xFEE00320;
	timerManager.initialCount = (UINT32*)(ADDRESS)0xFEE00380;
	timerManager.currentCount = (UINT32*)(ADDRESS)0xFEE00390;
	timerManager.divideConfiguration = (UINT32*)(ADDRESS)0xFEE003E0;

	timerManager.CreateTimer = _CreateTimer;
	timerManager.StartTimer = _StartTimer;
	timerManager.CountTime = _CountTime;
	timerManager.StopTimer = _StopTimer;

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

STATIC VOID
DUMMYAPI _CreateTimer(
	OUT			TIMER_MANAGER*	_this,
	IN CONST	UINTN			_timeOut,
	IN CONST	INTN 			_value
)	{
	TIMER timer;

	timer.timeOut = _timeOut;
	timer.value = _value;

	_this->timersQueue->Push(_this->timersQueue, timer);
	
	return ;
}

STATIC VOID
DUMMYAPI _StartTimer(
	OUT	TIMER_MANAGER*	_this
)	{
	*_this->initialCount = MAX_COUNT;
}

STATIC UINT32
DUMMYAPI _CountTime(
	IN CONST	TIMER_MANAGER*	_this
)	{
	return MAX_COUNT - *_this->currentCount;
}

STATIC VOID
DUMMYAPI _StopTimer(
	OUT	TIMER_MANAGER*	_this
)	{
	*_this->initialCount = 0;
}

BOOLEAN
DUMMYAPI TimerOnInterrupt(
	VOID
)   {
	BOOLEAN         isTimeOut = FALSE;

	MESSAGE         message;
	TIMER           prioritizedTimer;
	TIMER			timer;

	MESSAGE_QUEUE*	messageQueue = GetMessageQueue();

	++timerManager.tick;

	for(;;) {
		prioritizedTimer = *timerManager.timersQueue->Top(timerManager.timersQueue);

		if (prioritizedTimer.timeOut > timerManager.tick) {
			break;
		}

		if (prioritizedTimer.value == taskTimerValue) {
			isTimeOut = TRUE;
			prioritizedTimer = timerManager.timersQueue->Pop(timerManager.timersQueue);
			timer.timeOut = timerManager.tick + taskTimerPeriod;
			timer.value = taskTimerValue;
			timerManager.timersQueue->Push(timerManager.timersQueue, timer);
			continue;
		}
		
		message.type = TimerInterruptIndex;
		message.argument.timer.timeOut = prioritizedTimer.timeOut;
		message.argument.timer.value = prioritizedTimer.value;

		messageQueue->Push(messageQueue, message);
	}

	return isTimeOut;
}

VOID
DUMMYAPI WaitMilliSeconds(
	IN CONST	UINT32	_milliSeconds
)	{
	CONST	UINT32							PMTimerFreqeuncy = 3579545;
			BOOLEAN							PMTimer32;
			UINT16							PMTimerPort;
			UINT32							start;
			UINT32							end;

			FIXED_ACPI_DESCRIPTION_TABLE*	FADT = GetFADT();

	PMTimer32 = (FADT->flags >> 8) & 1;
	PMTimerPort = FADT->PMTimerBlock & 0xFFFF;
	start = IOIn32(PMTimerPort);
	end = start + PMTimerFreqeuncy * _milliSeconds / 1000;

	if (PMTimer32 != TRUE) {
		end &= 0x00FFFFFFu;
	}

	if (end < start) {
		while (IOIn32(PMTimerPort) >= start);
	}
	while (IOIn32(PMTimerPort) < end);

	return;
}