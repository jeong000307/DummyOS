#ifndef __ASSEMBLYFUNCTION_H__
#define __ASSEMBLYFUNCTION_H__

#include "Type.h"

extern void Enter(void);

extern void Test(uint64 value);

extern void Pause(void);

/**
    
**/

extern void IOOut32(
  uint16 address, 
  uint32 data);

/**
    
**/

extern uint32 IOIn32(
  uint16 address);

/**
    
**/

extern void LoadGDT(
  uint16 limit, 
  uint64 offset);

/**

**/

extern void LoadIDT(
  uint16 limit,
  uint64 offset);

/**

**/

extern void InitializeSegmentRegister(
  uint16 csValue,
  uint16 ssValue,
  uint16 anotherValue);

extern uint16 GetCS(void);

extern uint16 GetSS(void);

extern uint64 GetCR3(void);

extern void SetCR3(
  uint64 value);

extern void SwitchContext(
  void* nextContext,
  void* currentContext);

extern void ClearInterruptFlag(void);

extern void SetInterruptFlag(void);

extern void TimerInterruptHandler(void);

#endif