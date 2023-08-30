#include "DataStructure.h"

void InitializeMessageQueue(
  MESSAGE_QUEUE* meesageQueue,
  size           capacity) {
    meesageQueue->count = 0;
    meesageQueue->capacity = capacity;

    meesageQueue->messages = AllocateMemory(meesageQueue->capacity * sizeof(struct Message));

    meesageQueue->Push = __MessageQueuePush;
    meesageQueue->Pop = __MessageQueuePop;

    meesageQueue->front = 0;
    meesageQueue->end = 0;

    return;
}

static code __MessageQueuePush(
  MESSAGE_QUEUE* messageQueue,
  struct Message value) {
    if (messageQueue->count >= messageQueue->capacity) {
        return QUEUE_ERROR;
    }

    messageQueue->messages[messageQueue->end++] = value;
    ++messageQueue->count;

    if (messageQueue->end >= messageQueue->capacity) {
        messageQueue->end = 0;
    }

    return SUCCESS;
}

static struct Message __MessageQueuePop(
  MESSAGE_QUEUE* messageQueue) {
    struct Message value;

    if (messageQueue->count <= 0) {
        return;
    }

    value = messageQueue->messages[messageQueue->front++];
    --messageQueue->count;

    if (messageQueue->front >= messageQueue->capacity) {
        messageQueue->front = 0;
    }

    return value;
}

void InitializeTimerQueue(
  TIMER_QUEUE* this,
  size         capacity) {
    this->count = 0;
    this->capacity = capacity;

    this->timers = AllocateMemory((this->capacity + 1) * sizeof(struct Timer));

    this->Push = __TimerQueuePush;
    this->Pop = __TimerQueuePop;
    this->Top = __TimerQueueTop;

    return;
}

static code __TimerQueuePush(
  TIMER_QUEUE*  this,
  struct Timer value) {
    size index;
    size parentIndex;

    if (this->count >= this->capacity) {
        return TIMER_ERROR;
    }

    index = ++this->count;
    parentIndex = index / 2;

    while (index > 1 and value.timeOut < (this->timers[parentIndex]).timeOut) {
        //Test(value->timeOut);
        this->timers[index] = this->timers[parentIndex];

        index /= 2;
        parentIndex = index / 2;
    }

    this->timers[index] = value;

    return SUCCESS;
}

static struct Timer __TimerQueuePop(
  TIMER_QUEUE* this) {
    size index = 1;
    size left;
    size right;
    size small;

    struct Timer buffer;
    struct Timer value;

    if (this->count <= 0) {
        return;
    }

    value = this->timers[index];

    this->timers[index] = this->timers[this->count--];

    while (index < this->count) {
        left = 2 * index;
        right = 2 * index + 1;
        small = left;

        if (right < this->count and this->timers[left].timeOut > this->timers[right].timeOut) {
            small = right;
        }

        if (this->timers[index].timeOut > this->timers[small].timeOut) {
            buffer = this->timers[index];
            this->timers[index] = this->timers[small];
            this->timers[small] = buffer;
        }

        index = small;
    }

    return value;
}

static struct Timer* __TimerQueueTop(
  TIMER_QUEUE* timerQueue) {
    return &(timerQueue->timers[1]);
}

//void InitializeQueue(
//  QUEUE* queue,
//  byte type, 
//  size capacity) {
//    queue->count = 0;
//    queue->capacity = capacity;
//
//    if (capacity < 1) {
//        return;
//    }
//
//    switch (type) {
//        case 'b':
//            queue->data = (int8*){ 0 };
//            queue->Push = __QueuePushByte;
//            queue->Pop = __QueuePopByte;
//            break;
//        case 'w': 
//            queue->data = (int16*){ 0 };
//            queue->Push = __QueuePushWord;
//            queue->Pop = __QueuePopWord;
//            break;
//        case 'd': 
//            queue->data = (int32*){ 0 };
//            queue->Push = __QueuePushDword;
//            queue->Pop = __QueuePopDword;
//            break;
//        case 'q': 
//            queue->data = (int64*){ 0 };
//            queue->Push = __QueuePushQword;
//            queue->Pop = __QueuePopQword;
//            break;
//        default:
//            return ;
//    }
//
//    queue->front = &queue->data;
//    queue->end = &queue->data;
//
//    return ;
//}
//
//static code __QueuePushByte(
//  QUEUE* queue, 
//  int8 value){
//    if (queue->count >= queue->capacity) {
//        return QUEUE_ERROR;
//    }
//
//    *(int8*)queue->end = value;
//
//    ++(int8*)queue->end;
//    ++queue->count;
//
//    if (queue->end >= (int8*)queue->data + queue->capacity) {
//        queue->end = (addr)queue->data;
//    }
//
//    return SUCCESS;
//}
//
//static int8 __QueuePopByte(
//  QUEUE* queue) {
//    int8 value;
//
//    if (queue->count <= 0) {
//        return ;
//    }
//
//    value = *(int8*)queue->front;
//
//    ++(int8*)queue->front;
//
//    if (queue->front >= (int8*)queue->data + queue->capacity) {
//        queue->front = (addr)queue->data;
//    }
//
//    --queue->count;
//
//    return value;
//}
//
//static code __QueuePushWord(
//  QUEUE* queue, 
//  int16 value) {
//    if (queue->count >= queue->capacity) {
//        return QUEUE_ERROR;
//    }
//
//    *(int16*)queue->end = value;
//
//    ++(int16*)queue->end;
//    ++queue->count;
//
//    if (queue->end >= (int16*)queue->data + queue->capacity) {
//        queue->end = (addr)queue->data;
//    }
//
//    return SUCCESS;
//}
//
//static int16 __QueuePopWord(
//  QUEUE* queue) {
//    int16 value;
//
//    if (queue->count <= 0) {
//        return;
//    }
//
//    value = *(int16*)queue->front;
//
//    ++(int16*)queue->front;
//
//    if (queue->front >= (int16*)queue->data + queue->capacity) {
//        queue->front = (addr)queue->data;
//    }
//
//    --queue->count;
//
//    return value;
//}
//
//static code __QueuePushDword(
//  QUEUE* queue, 
//  int32 value) {
//    if (queue->count >= queue->capacity) {
//        return QUEUE_ERROR;
//    }
//
//    *(int32*)queue->end = value;
//
//    ++(int32*)queue->end;
//    ++queue->count;
//
//    if (queue->end >= (int32*)queue->data + queue->capacity) {
//        queue->end = (addr)queue->data;
//    }
//
//    return SUCCESS;
//}
//
//static int32 __QueuePopDword(
//  QUEUE* queue) {
//    int32 value;
//
//    if (queue->count <= 0) {
//        return;
//    }
//
//    value = *(int32*)queue->front;
//
//    ++(int32*)queue->front;
//
//    if (queue->front >= (int32*)queue->data + queue->capacity) {
//        queue->front = (addr)queue->data;
//    }
//
//    --queue->count;
//
//    return value;
//}
//
//static code __QueuePushQword(
//  QUEUE* queue, 
//  int64 value) {
//    if (queue->count >= queue->capacity) {
//        return QUEUE_ERROR;
//    }
//
//    *(int64*)queue->end = value;
//
//    ++(int64*)queue->end;
//    ++queue->count;
//
//    if (queue->end >= (int64*)queue->data + queue->capacity) {
//        queue->end = (addr)queue->data;
//    }
//
//    return SUCCESS;
//}
//
//static int64 __QueuePopQword(
//  QUEUE* queue) {
//    int64 value;
//
//    if (queue->count <= 0) {
//        return;
//    }
//
//    value = *(int64*)queue->front;
//
//    ++(int64*)queue->front;
//
//    if (queue->front >= (int64*)queue->data + queue->capacity) {
//        queue->front = (addr)queue->data;
//    }
//
//    --queue->count;
//
//    return value;
//}
//
//void InitializeStack(
//    STACK* stack,
//    byte type,
//    size capacity) {
//    stack->count = 0;
//    stack->capacity = capacity;
//
//    if (capacity < 1) {
//        return;
//    }
//
//    switch (type) {
//    case 'b':
//        stack->data = (int8*){ 0 };
//        stack->Push = __StackPushByte;
//        stack->Pop = __StackPopByte;
//        break;
//    case 'w':
//        stack->data = (int16*){ 0 };
//        stack->Push = __StackPushWord;
//        stack->Pop = __StackPopWord;
//        break;
//    case 'd':
//        stack->data = (int32*){ 0 };
//        stack->Push = __StackPushDword;
//        stack->Pop = __StackPopDword;
//        break;
//    case 'q':
//        stack->data = (int64*){ 0 };
//        stack->Push = __StackPushQword;
//        stack->Pop = __StackPopQword;
//        break;
//    default:
//        return;
//    }
//
//    stack->end = &stack->data;
//
//    return;
//}
//
//static code __StackPushByte(
//    STACK* stack,
//    int8 value) {
//    if (stack->count >= stack->capacity) {
//        return STACK_ERROR;
//    }
//
//    *(int8*)stack->end = value;
//
//    ++(int8*)stack->end;
//    ++stack->count;
//
//    return SUCCESS;
//}
//
//static int8 __StackPopByte(
//    STACK* stack) {
//    int8 value;
//
//    if (stack->count <= 0) {
//        return;
//    }
//
//    value = *(int8*)stack->end;
//
//    --(int8*)stack->end;
//
//    return value;
//}
//
//static code __StackPushWord(
//    STACK* stack,
//    int16 value) {
//    if (stack->count >= stack->capacity) {
//        return STACK_ERROR;
//    }
//
//    *(int16*)stack->end = value;
//
//    ++(int16*)stack->end;
//    ++stack->count;
//
//    return SUCCESS;
//}
//
//static int16 __StackPopWord(
//    STACK* stack) {
//    int16 value;
//
//    if (stack->count <= 0) {
//        return;
//    }
//
//    value = *(int16*)stack->end;
//
//    --(int16*)stack->end;
//
//    return value;
//}
//
//static code __StackPushDword(
//    STACK* stack,
//    int32 value) {
//    if (stack->count >= stack->capacity) {
//        return STACK_ERROR;
//    }
//
//    *(int32*)stack->end = value;
//
//    ++(int32*)stack->end;
//    ++stack->count;
//
//    return SUCCESS;
//}
//
//static int32 __StackPopDword(
//    STACK* stack) {
//    int32 value;
//
//    if (stack->count <= 0) {
//        return;
//    }
//
//    value = *(int32*)stack->end;
//
//    --(int32*)stack->end;
//
//    return value;
//}
//
//static code __StackPushQword(
//    STACK* stack,
//    int64 value) {
//    if (stack->count >= stack->capacity) {
//        return STACK_ERROR;
//    }
//
//    *(int64*)stack->end = value;
//
//    ++(int64*)stack->end;
//    ++stack->count;
//
//    return SUCCESS;
//}
//
//static int64 __StackPopQword(
//    STACK* stack) {
//    int64 value;
//
//    if (stack->count <= 0) {
//        return;
//    }
//
//    value = *(int64*)stack->end;
//
//    --(int64*)stack->end;
//
//    return value;
//}