// 나중에 시스템 애플리케이션으로 이전할 계획

#pragma once

#include "Error.h"
#include "Graphics.h"
#include "Memory.h"
#include "String.h"
#include "Type.h"

#define CONSOLE_ROW    25
#define CONSOLE_COLUMN 80

typedef struct __CONSOLE CONSOLE;

typedef void (*PRINT)(
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
  SCREEN* screen);

/**
    
**/

static void __Print(
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