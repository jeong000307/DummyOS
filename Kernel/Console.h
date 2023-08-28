#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Error.h"
#include "Graphics.h"
#include "Memory.h"
#include "String.h"
#include "Type.h"

#define CONSOLE_ROW    25
#define CONSOLE_COLUMN 80

typedef struct __CONSOLE CONSOLE;

typedef code (*PRINT)(
  CONSOLE*        this,
  const byte*     string,
  ...);

struct Cursor {
    size x;
    size y;
};

struct __CONSOLE {
    size              rows;
    size              columns;

    struct Cursor     cursor;
    struct PixelColor foregroundColor;
    struct PixelColor backgroundColor;

    SCREEN*           screen;

    PRINT             Print;
};

CONSOLE* GetSystemConsole(void);

code InitializeSystemConsole(
  SCREEN*         screen);

/**
    
**/

static code __Print(
  CONSOLE*        this,
  const byte*     string,
  ...);

/**

**/

static code WriteAscii(
  const SCREEN* screen,
  size                    x,
  size                    y,
  byte                    character,
  const struct PixelColor color);

/**

**/

#endif