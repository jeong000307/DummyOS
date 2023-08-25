#ifndef __ASSEMBLYFUNCTION_H__
#define __ASSEMBLYFUNCTION_H__

#include "Type.h"

extern void Halt(void);

extern void IOOut32(
  uint16 address, 
  uint32 data);

extern uint32 IOIn32(
  uint16 address);

#endif