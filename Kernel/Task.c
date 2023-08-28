#include "Task.h"

static TASK_MANAGER taskManager;

TASK_MANAGER* GetTaskManager(void) {
    return &taskManager;
}

code InitializeTaskManager(void) {
    taskManager.CreateTask = __CreateTask;
    taskManager.SwitchTask = __SwitchTask;

    ClearInterruptFlag();
    SetInterruptFlag();

    return SUCCESS;
}

static void InitializeTask(
  struct Task* this,
  addr function, 
  int64 data) {
    addr stackEnd = &this->stack[STACK_SIZE];

    SetMemory(this->stack, 0, sizeof(this->stack));

    SetMemory(&this->context, 0, sizeof(struct Context));
    this->context.CR3 = GetCR3();
    this->context.RFLAGS = 0x202;
    this->context.CS = GetCS();
    this->context.SS = GetSS();
    this->context.RSP = (stackEnd & ~0xflu) - 8;
    this->context.RIP = function;
    this->context.RDI = this->id;
    this->context.RSI = data;

    *(uint32 *)(&this->context.FXSAVE[24]) = 0x1f80;
}

static void __CreateTask(
  TASK_MANAGER* this,
  addr function, 
  int64 data) {
    struct Task task;

    InitializeTask(&task, function, data);

    ++this->latestID;

    this->tasks[task.id] = task;
}

static void __SwitchTask(
  TASK_MANAGER* this) {
    size nextTaskIndex = this->latestID + 1;
    struct Task currentTask;
    struct Task nextTask;

    if (nextTaskIndex >= STACK_SIZE) {
        nextTaskIndex = 0;
    }

    currentTask = this->tasks[this->latestID];
    nextTask = this->tasks[nextTaskIndex];
    this->latestID = nextTaskIndex;

    SwitchContext(&nextTask.context, &currentTask.context);
}