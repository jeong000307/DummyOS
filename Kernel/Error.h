#ifndef __ERROR_H__
#define __ERROR_H__

#include "Type.h"

#define SUCCESS           0
#define INVALID_PARAMETER 1
#define MEMORY_ERROR      2
#define PCI_ERROR         3
#define QUEUE_ERROR       4
#define STACK_ERROR       5

char* Error(
  code status);

#endif