#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "ErrorCode.h"
#include "Graphics.h"
#include "Memory.h"
#include "String.h"
#include "Type.h"

typedef struct __SYSTEM_CONSOLE SYSTEM_CONSOLE;

typedef code (*PRINT)(
  IN SYSTEM_CONSOLE* this,
  IN const byte*     string,
  IN ...);

struct Cursor {
    size x;
    size y;
};

struct __SYSTEM_CONSOLE {
    size              rows;
    size              columns;

    struct Cursor     cursor;
    struct PixelColor foregroundColor;
    struct PixelColor backgroundColor;

    SCREEN*           screen;

    PRINT             Print;
};

SYSTEM_CONSOLE* GetSystemConsole(void);

code InitializeSystemConsole(
  IN  SCREEN*         screen);

/**
    
**/

static code __Print(
  IN SYSTEM_CONSOLE* this,
  IN const byte*     string,
  IN ...
);

/**

**/

static code WriteAscii(
  IN OUT const SCREEN* screen,
  IN     size                    x,
  IN     size                    y,
  IN     byte                    character,
  IN     const struct PixelColor color
);

/**

**/

#endif