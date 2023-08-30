#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

#include "Error.h"
#include "Memory.h"
#include "Type.h"

typedef struct __MESSAGE_QUEUE MESSAGE_QUEUE;

typedef code (*MESSAGE_QUEUE_PUSH)(
  MESSAGE_QUEUE* this,
  struct Message value);

typedef struct Message (*MESSAGE_QUEUE_POP)(
  MESSAGE_QUEUE* this);

struct __MESSAGE_QUEUE {
    size               count;
    size               capacity;
    size               front;
    size               end;

    struct Message*    messages;

    MESSAGE_QUEUE_PUSH Push;
    MESSAGE_QUEUE_POP  Pop;
};

struct Message {
    byte type;

    union {
        struct {
            size  timeOut;
            int64 value;
        } timer;
    } argument;
};

typedef struct __TIMER_QUEUE TIMER_QUEUE;

typedef code (*TIMER_QUEUE_PUSH)(
  TIMER_QUEUE* this,
  struct Timer value);

typedef struct Timer (*TIMER_QUEUE_POP)(
  TIMER_QUEUE* this);

typedef struct Timer* (*TIMER_QUEUE_TOP)(
  TIMER_QUEUE* this);

struct __TIMER_QUEUE {
    size             count;
    size             capacity;

    struct Timer     *timers;

    TIMER_QUEUE_PUSH Push;
    TIMER_QUEUE_POP  Pop;
    TIMER_QUEUE_TOP  Top;
};

struct Timer {
    size  timeOut;
    int64 value;
};

void InitializeMessageQueue(
  MESSAGE_QUEUE* meesageQueue,
  size           capacity);

static code __MessageQueuePush(
  MESSAGE_QUEUE* messageQueue,
  struct Message value);

static struct Message __MessageQueuePop(
  MESSAGE_QUEUE* messageQueue);

void InitializeTimerQueue(
  TIMER_QUEUE* this,
  size         capacity);

static code __TimerQueuePush(
  TIMER_QUEUE*  this,
  struct Timer value);

static struct Timer __TimerQueuePop(
  TIMER_QUEUE* this);

static struct Timer* __TimerQueueTop(
  TIMER_QUEUE* this);

//typedef struct __QUEUE QUEUE;
//typedef struct __STACK STACK;
//
//typedef code (*QUEUE_PUSH)(
//  QUEUE* this,
//  void* value);
//
//typedef void* (*QUEUE_POP)(
//  QUEUE* this);
//
//typedef code (*STACK_PUSH)(
//  STACK* this,
//  void* value);
//
//typedef void* (*STACK_POP)(
//  STACK* this);
//
//struct __QUEUE {
//    size       count;
//    size       capacity;
//
//    addr       front;
//    addr       end;
//
//    void*      data;
//
//    QUEUE_PUSH Push;
//    QUEUE_POP  Pop;
//};
//
//struct __STACK {
//    size       count;
//    size       capacity;
//
//    addr       end;
//
//    void*      data;
//
//    STACK_PUSH Push;
//    STACK_POP  Pop;
//};
//
//void InitializeQueue(
//    QUEUE* queue,
//    byte type,
//    size capacity);
//
//static code __QueuePushByte(
//    QUEUE* queue,
//    int8  value);
//
//static int8 __QueuePopByte(
//    QUEUE* queue);
//
//static code __QueuePushWord(
//    QUEUE* queue,
//    int16 value);
//
//static int16 __QueuePopWord(
//    QUEUE* queue);
//
//static code __QueuePushDword(
//    QUEUE* queue,
//    int32 value);
//
//static int32 __QueuePopDword(
//    QUEUE* queue);
//
//static code __QueuePushQword(
//    QUEUE* queue,
//    int64 value);
//
//static int64 __QueuePopQword(
//    QUEUE* queue);
//
//void InitializeStack(
//    STACK* stack,
//    byte type,
//    size capacity);
//
//static code __StackPushByte(
//    STACK* stack,
//    int8 value);
//
//static int8 __StackPopByte(
//    STACK* stack);
//
//static code __StackPushWord(
//    STACK* stack,
//    int16 value);
//
//static int16 __StackPopWord(
//    STACK* stack);
//
//static code __StackPushDword(
//    STACK* stack,
//    int32 value);
//
//static int32 __StackPopDword(
//    STACK* stack);
//
//static code __StackPushQword(
//    STACK* stack,
//    int64 value);
//
//static int64 __StackPopQword(
//    STACK* stack);

#endif