#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "Error.h"
#include "Memory.h"
#include "Type.h"

typedef struct _MESSAGE_QUEUE MESSAGE_QUEUE;

typedef struct {
	BYTE			type;

	union {
		struct {
			UINTN   timeOut;
			INTN    value;
		}			timer;
	}				argument;
} MESSAGE;

struct _MESSAGE_QUEUE {
	UINTN		count;
	UINTN		capacity;
	UINTN		front;
	UINTN		end;

	MESSAGE*	messages;

	STATUS		(DUMMYAPI* Push) (
					OUT			MESSAGE_QUEUE*	_this,
					IN CONST	MESSAGE			_value
				);
	MESSAGE		(DUMMYAPI* Pop) (
					OUT	MESSAGE_QUEUE*	_this
				);
};

MESSAGE_QUEUE*
DUMMYAPI GetMessageQueue(
	VOID
);

VOID
DUMMYAPI InitializeMessageQueue(
	OUT			MESSAGE_QUEUE*	_meesageQueue,
	IN CONST	UINTN           _capacity
);

STATIC STATUS
DUMMYAPI _MessageQueuePush(
	OUT			MESSAGE_QUEUE*	_this,
	IN CONST	MESSAGE			_value
);

STATIC MESSAGE
DUMMYAPI _MessageQueuePop(
	OUT	MESSAGE_QUEUE*	_this
);

#endif