#include "Task.h"

STATIC TASK_MANAGER taskManager;

TASK_MANAGER*
DUMMYAPI GetTaskManager(
	VOID
)   {
	return &taskManager;
}

STATUS
DUMMYAPI InitializeTaskManager(
	VOID
)   {
	taskManager.CreateTask = _CreateTask;
	taskManager.SwitchTask = _SwitchTask;

	SetInterruptFlag(FALSE);
	SetInterruptFlag(TRUE);

	return SUCCESS;
}

STATIC VOID
DUMMYAPI InitializeTask(
	OUT			TASK*   _this,
	IN CONST	ADDRESS	_function,
	IN CONST	INT64	_data
)	{
	ADDRESS stackEnd = _this->stack[STACK_SIZE - 1];

	SetMemory(_this->stack, 0, sizeof(_this->stack));

	SetMemory(&_this->context, 0, sizeof(CONTEXT));
	_this->context.CR3 = GetCR3();
	_this->context.RFLAGS = 0x202;
	_this->context.CS = GetCS();
	_this->context.SS = GetSS();
	_this->context.RSP = (stackEnd & ~0xflu) - 8;
	_this->context.RIP = _function;
	_this->context.RDI = _this->id;
	_this->context.RSI = _data;

	*(UINT32*)(&_this->context.FXSAVE[24]) = 0x1f80;
}

STATIC VOID
DUMMYAPI _CreateTask(
	OUT			TASK_MANAGER*	_this,
	IN CONST	ADDRESS			_function,
	IN CONST	INT64			_data
)	{
	TASK    task;

	InitializeTask(&task, _function, _data);

	++_this->latestID;

	CopyMemory(&task, &_this->tasks[task.id], sizeof(task));
}

STATIC VOID
DUMMYAPI _SwitchTask(
	OUT	TASK_MANAGER*	_this
)	{
	UINTN   nextTaskIndex = _this->latestID + 1;
	TASK    currentTask;
	TASK    nextTask;

	if (nextTaskIndex >= STACK_SIZE) {
		nextTaskIndex = 0;
	}

	CopyMemory(&_this->tasks[_this->latestID], &currentTask, sizeof(_this->tasks[_this->latestID]));
	CopyMemory(&_this->tasks[nextTaskIndex], &nextTask, sizeof(_this->tasks[nextTaskIndex]));
	_this->latestID = nextTaskIndex;

	SwitchContext(&nextTask.context, &currentTask.context);
}