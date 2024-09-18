#include "Message.h"

STATIC	MESSAGE_QUEUE	messageQueue;

MESSAGE_QUEUE*
DUMMYAPI GetMessageQueue(
	VOID
)	{
	return &messageQueue;
}

VOID
DUMMYAPI InitializeMessageQueue(
	OUT			MESSAGE_QUEUE*  _meesageQueue,
	IN CONST	UINTN           _capacity
)   {
	_meesageQueue->count = 0;
	_meesageQueue->capacity = _capacity;

	_meesageQueue->messages = AllocateMemory(_meesageQueue->capacity * sizeof(MESSAGE));

	_meesageQueue->Push = _MessageQueuePush;
	_meesageQueue->Pop = _MessageQueuePop;

	_meesageQueue->front = 0;
	_meesageQueue->end = 0;

	return;
}

STATIC STATUS
DUMMYAPI _MessageQueuePush(
	OUT			MESSAGE_QUEUE*  _this,
	IN CONST	MESSAGE			_value
)   {
	if (_this->count >= _this->capacity) {
		return ERR_DEVICE_ERROR;
	}

	_this->messages[_this->end++] = _value;
	++_this->count;

	if (_this->end >= _this->capacity) {
		_this->end = 0;
	}

	return SUCCESS;
}

STATIC MESSAGE
DUMMYAPI _MessageQueuePop(
	OUT	MESSAGE_QUEUE* _this
)   {
	MESSAGE value;

	if (_this->count <= 0) {
		Inspect(ERR_DEVICE_ERROR);
	}

	value = _this->messages[_this->front++];
	--_this->count;

	if (_this->front >= _this->capacity) {
		_this->front = 0;
	}

	return value;
}