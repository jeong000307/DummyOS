#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Graphics.h"
#include "Type.h"
#include "Memory.h"
#include "ErrorCode.h"
#include "String.h"

typedef struct __SYSTEM_CONSOLE SYSTEM_CONSOLE;

typedef code
    (*SYSTEM_PRINT)(
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

    byte              buffer[25][80]; // VLA issue; row, columns
    struct Cursor     cursor;
    struct PixelColor foregroundColor;
    struct PixelColor backgroundColor;

    SCREEN*           screen;

    SYSTEM_PRINT      SystemPrint;
};

code CreateSystemConsole(
  OUT SYSTEM_CONSOLE* this,
  IN  SCREEN*         screen
);

static code __SystemPrint(
  IN SYSTEM_CONSOLE* this,
  IN const byte*     string,
  IN ...
);

code WriteAscii(
  IN OUT const SCREEN* screen,
  IN     size                    x,
  IN     size                    y,
  IN     byte                    character,
  IN     const struct PixelColor color
);

#endif