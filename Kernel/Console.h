#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Graphics.h"

static const char* GetFont(
  IN char character);

static void WriteAscii(
  IN OUT const SCREEN* screen,
  IN     uint32_t                x,
  IN     uint32_t                y,
  IN     char                    character,
  IN     const struct PixelColor color);

void WriteString(
  IN OUT const SCREEN* screen,
  IN     uint32_t                x,
  IN     uint32_t                y,
  IN     const char* string,
  IN     const struct PixelColor color);

#endif