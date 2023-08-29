#ifndef __TASK_H__
#define __TASK_H__

#include "AssemblyFunction.h"
#include "Memory.h"
#include "Type.h"

#define STACK_BYTE 4096
#define STACK_SIZE (STACK_BYTE / sizeof(uint64))

typedef struct __TASK_MANAGER TASK_MANAGER;

typedef void (*CREATE_TASK)(
  TASK_MANAGER* this,
  addr          function,
  int64         data);

typedef void (*SWITCH_TASK)(
  TASK_MANAGER* this);

struct Context {
    uint64 CR3, RIP, RFLAGS, Reserved1;
    uint64 CS, SS, FS, GS;
    uint64 RAX, RBX, RCX, RDX, RDI, RSI, RSP, RBP;
    uint64 R8, R9, R10, R11, R12, R13, R14, R15;
    byte FXSAVE[512];
};

struct Task {
    uint64                      id;

    uint64                      stack[STACK_SIZE];
    struct Context _Alignas(16) context;
};

struct __TASK_MANAGER {
    size          latestID;
    struct Task*  tasks;

    CREATE_TASK   CreateTask;
    SWITCH_TASK   SwitchTask;
};

TASK_MANAGER* GetTaskManager(void);

code InitializeTaskManager(void);

static void InitializeTask(
  struct Task* this,
  addr function, 
  int64 data);

static void __CreateTask(
  TASK_MANAGER* this,
  addr          function,
  int64         data);

static void __SwitchTask(
  TASK_MANAGER* this);

#endif