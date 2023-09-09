#pragma once

#include "Type.h"

extern void Enter32(void);

extern void Enter64(void);

extern void Assert(uint64 value);

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

extern void EnableApplicationProcessor(void);

extern void SetInterruptFlag(bool flag);

extern void TimerInterruptHandler(void);