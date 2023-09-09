#ifndef __TYPE_H__
#define __TYPE_H__

#include "Error.h"

#define MAX_INT8    0x7F
#define MAX_INT16   0x7FFF
#define MAX_INT32   0x7FFFFFFF
#define MAX_INT64   0x7FFFFFFFFFFFFFFF
#define MAX_UINT8   0xFF
#define MAX_UINT16  0xFFFF
#define MAX_UINT32  0xFFFFFFFF
#define MAX_UINT64  0xFFFFFFFFFFFFFFFF

#define MIN_INT8    (-MAX_INT8 - 1)
#define MIN_INT16   (-MAX_INT16 - 1)
#define MIN_INT32   (-MAX_INT32 - 1)
#define MIN_INT64   (-MAX_INT64 - 1)

#define NULL  (void*)0

#define false 0
#define true  1

#define or  ||
#define and &&

#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_arg(list, type)    (*(type *)((list += sizeof(type)) - sizeof(type)))
#define va_end(param)         (param = (va_list)0)

#define KiB(param)          (param * 1024)
#define MiB(param)          KiB(param * 1024)
#define GiB(param)          MiB(param * 1024)

typedef unsigned __int64 addr;
typedef signed __int8    byte;
typedef signed __int8    code;
typedef unsigned __int64 size;
typedef unsigned __int64 frame;
typedef unsigned __int32 bitmap;

typedef _Bool            bool;
typedef signed __int8    int8;
typedef __int16          int16;
typedef __int32          int32;
typedef __int64          int64;
typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned __int8* va_list;

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

typedef code(*MESSAGE_QUEUE_PUSH)(
    MESSAGE_QUEUE* this,
    struct Message value);

typedef struct Message(*MESSAGE_QUEUE_POP)(
    MESSAGE_QUEUE* this);

struct __MESSAGE_QUEUE {
    size               count;
    size               capacity;
    size               front;
    size               end;

    struct Message* messages;

    MESSAGE_QUEUE_PUSH Push;
    MESSAGE_QUEUE_POP  Pop;
};

typedef struct __TIMER_QUEUE TIMER_QUEUE;

typedef code(*TIMER_QUEUE_PUSH)(
    TIMER_QUEUE* this,
    struct Timer value);

typedef struct Timer(*TIMER_QUEUE_POP)(
    TIMER_QUEUE* this);

typedef struct Timer* (*TIMER_QUEUE_TOP)(
    TIMER_QUEUE* this);

struct __TIMER_QUEUE {
    size             count;
    size             capacity;

    struct Timer* timers;

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
    TIMER_QUEUE* this,
    struct Timer value);

static struct Timer __TimerQueuePop(
    TIMER_QUEUE* this);

static struct Timer* __TimerQueueTop(
    TIMER_QUEUE* this);

#endif