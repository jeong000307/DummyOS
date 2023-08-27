#ifndef __ASSEMBLYFUNCTION_H__
#define __ASSEMBLYFUNCTION_H__

#include "Type.h"

extern void Enter(void);

extern void Test(uint64 value);

extern void Pause(void);

/**
    
**/

extern void IOOut32(
  IN uint16 address, 
  IN uint32 data);

/**
    
**/

extern uint32 IOIn32(
  IN uint16 address);

/**
    
**/

extern void LoadGDT(
  IN uint16 limit, 
  IN uint64 offset);

/**

**/

extern void LoadIDT(
  IN uint16 limit,
  IN uint64 offset);

/**

**/

extern void InitializeSegmentRegister(
  IN uint16 csValue,
  IN uint16 ssValue,
  IN uint16 anotherValue);

extern uint16 GetCS(void);

extern void SetCR3(
  IN uint64 value);

#endif