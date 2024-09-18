#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "Error.h"
#include "Type.h"

typedef enum {
	upper8Bytes = 0,
	LDT = 2,
	TSSAvailable = 9,
	TSSBusy = 11,
	CallGate = 12,
	interruptGate = 14,
	trapGate = 15,

	readWrite = 2,
	executeRead = 10
} DESCRIPTOR_TYPE;

#endif