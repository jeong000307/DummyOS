#include "DataStructure.h"

void InitializeQueue(
  QUEUE* queue,
  byte type, 
  size capacity) {
    queue->count = 0;
    queue->capacity = capacity;

    if (capacity < 1) {
        return;
    }

    switch (type) {
        case 'b':
            queue->data = (int8*){ 0 };
            queue->Push = __QueuePushByte;
            queue->Pop = __QueuePopByte;
            break;
        case 'w': 
            queue->data = (int16*){ 0 };
            queue->Push = __QueuePushWord;
            queue->Pop = __QueuePopWord;
            break;
        case 'd': 
            queue->data = (int32*){ 0 };
            queue->Push = __QueuePushDword;
            queue->Pop = __QueuePopDword;
            break;
        case 'q': 
            queue->data = (int64*){ 0 };
            queue->Push = __QueuePushQword;
            queue->Pop = __QueuePopQword;
            break;
        default:
            return ;
    }

    queue->front = &queue->data;
    queue->end = &queue->data;

    return ;
}

static code __QueuePushByte(
  QUEUE* queue, 
  int8 value){
    if (queue->count >= queue->capacity) {
        return QUEUE_ERROR;
    }

    *(int8*)queue->end = value;

    ++(int8*)queue->end;
    ++queue->count;

    if (queue->end >= (int8*)queue->data + queue->capacity) {
        queue->end = (addr)queue->data;
    }

    return SUCCESS;
}

static int8 __QueuePopByte(
  QUEUE* queue) {
    int8 value;

    if (queue->count <= 0) {
        return ;
    }

    value = *(int8*)queue->front;

    ++(int8*)queue->front;

    if (queue->front >= (int8*)queue->data + queue->capacity) {
        queue->front = (addr)queue->data;
    }

    return value;
}

static code __QueuePushWord(
  QUEUE* queue, 
  int16 value) {
    if (queue->count >= queue->capacity) {
        return QUEUE_ERROR;
    }

    *(int16*)queue->end = value;

    ++(int16*)queue->end;
    ++queue->count;

    if (queue->end >= (int16*)queue->data + queue->capacity) {
        queue->end = (addr)queue->data;
    }

    return SUCCESS;
}

static int16 __QueuePopWord(
  QUEUE* queue) {
    int16 value;

    if (queue->count <= 0) {
        return;
    }

    value = *(int16*)queue->front;

    ++(int16*)queue->front;

    if (queue->front >= (int16*)queue->data + queue->capacity) {
        queue->front = (addr)queue->data;
    }

    return value;
}

static code __QueuePushDword(
  QUEUE* queue, 
  int32 value) {
    if (queue->count >= queue->capacity) {
        return QUEUE_ERROR;
    }

    *(int32*)queue->end = value;

    ++(int32*)queue->end;
    ++queue->count;

    if (queue->end >= (int32*)queue->data + queue->capacity) {
        queue->end = (addr)queue->data;
    }

    return SUCCESS;
}

static int32 __QueuePopDword(
  QUEUE* queue) {
    int32 value;

    if (queue->count <= 0) {
        return;
    }

    value = *(int32*)queue->front;

    ++(int32*)queue->front;

    if (queue->front >= (int32*)queue->data + queue->capacity) {
        queue->front = (addr)queue->data;
    }

    return value;
}

static code __QueuePushQword(
  QUEUE* queue, 
  int64 value) {
    if (queue->count >= queue->capacity) {
        return QUEUE_ERROR;
    }

    *(int64*)queue->end = value;

    ++(int64*)queue->end;
    ++queue->count;

    if (queue->end >= (int64*)queue->data + queue->capacity) {
        queue->end = (addr)queue->data;
    }

    return SUCCESS;
}

static int64 __QueuePopQword(
  QUEUE* queue) {
    int64 value;

    if (queue->count <= 0) {
        return;
    }

    value = *(int64*)queue->front;

    ++(int64*)queue->front;

    if (queue->front >= (int64*)queue->data + queue->capacity) {
        queue->front = (addr)queue->data;
    }

    return value;
}

void InitializeStack(
    STACK* stack,
    byte type,
    size capacity) {
    stack->count = 0;
    stack->capacity = capacity;

    if (capacity < 1) {
        return;
    }

    switch (type) {
    case 'b':
        stack->data = (int8*){ 0 };
        stack->Push = __StackPushByte;
        stack->Pop = __StackPopByte;
        break;
    case 'w':
        stack->data = (int16*){ 0 };
        stack->Push = __StackPushWord;
        stack->Pop = __StackPopWord;
        break;
    case 'd':
        stack->data = (int32*){ 0 };
        stack->Push = __StackPushDword;
        stack->Pop = __StackPopDword;
        break;
    case 'q':
        stack->data = (int64*){ 0 };
        stack->Push = __StackPushQword;
        stack->Pop = __StackPopQword;
        break;
    default:
        return;
    }

    stack->end = &stack->data;

    return;
}

static code __StackPushByte(
    STACK* stack,
    int8 value) {
    if (stack->count >= stack->capacity) {
        return STACK_ERROR;
    }

    *(int8*)stack->end = value;

    ++(int8*)stack->end;
    ++stack->count;

    return SUCCESS;
}

static int8 __StackPopByte(
    STACK* stack) {
    int8 value;

    if (stack->count <= 0) {
        return;
    }

    value = *(int8*)stack->end;

    --(int8*)stack->end;

    return value;
}

static code __StackPushWord(
    STACK* stack,
    int16 value) {
    if (stack->count >= stack->capacity) {
        return STACK_ERROR;
    }

    *(int16*)stack->end = value;

    ++(int16*)stack->end;
    ++stack->count;

    return SUCCESS;
}

static int16 __StackPopWord(
    STACK* stack) {
    int16 value;

    if (stack->count <= 0) {
        return;
    }

    value = *(int16*)stack->end;

    --(int16*)stack->end;

    return value;
}

static code __StackPushDword(
    STACK* stack,
    int32 value) {
    if (stack->count >= stack->capacity) {
        return STACK_ERROR;
    }

    *(int32*)stack->end = value;

    ++(int32*)stack->end;
    ++stack->count;

    return SUCCESS;
}

static int32 __StackPopDword(
    STACK* stack) {
    int32 value;

    if (stack->count <= 0) {
        return;
    }

    value = *(int32*)stack->end;

    --(int32*)stack->end;

    return value;
}

static code __StackPushQword(
    STACK* stack,
    int64 value) {
    if (stack->count >= stack->capacity) {
        return STACK_ERROR;
    }

    *(int64*)stack->end = value;

    ++(int64*)stack->end;
    ++stack->count;

    return SUCCESS;
}

static int64 __StackPopQword(
    STACK* stack) {
    int64 value;

    if (stack->count <= 0) {
        return;
    }

    value = *(int64*)stack->end;

    --(int64*)stack->end;

    return value;
}