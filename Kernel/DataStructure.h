#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

#include "Error.h"
#include "Memory.h"
#include "Type.h"

struct Message {
    byte type;

    union {
        struct {
            size  timeOut;
            int64 value;
        } timer;
    } argument;
};

struct Timer {
    size  timeOut;
    int64 value;
};

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

#endif