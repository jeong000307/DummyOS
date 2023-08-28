#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "Error.h"
#include "Memory.h"
#include "Type.h"

typedef struct __SCREEN SCREEN;

typedef void (*WRITE_PIXEL)(
  const SCREEN*           this,
  size                    x,
  size                    y,
  const struct PixelColor color);

typedef byte* (*GET_PIXEL_ADDRESS)(
  const SCREEN* this,
  size          x,
  size          y);

struct PixelColor {
    byte red;
    byte green;
    byte blue;
};

enum PixelFormat {
    pixelRGBReserved8BitPerColor,
    pixelBGRReserved8BitPerColor
};

struct FrameBufferConfig {
    uint32            pixelsPerScanLine;
    uint32            horizontalResolution;
    uint32            verticalResolution;

    enum PixelFormat  pixelFormat;
    byte*             frameBuffer;
};

struct __SCREEN {
    uint32            pixelsPerScanLine;
    uint32            horizontalResolution;
    uint32            verticalResolution;

    byte*             frameBuffer;

    WRITE_PIXEL       WritePixel;
    GET_PIXEL_ADDRESS GetPixelAddress;
};

SCREEN* GetScreen(void);

code InitializeScreen(
  const struct FrameBufferConfig* frameBufferConfig);

static void __WritePixelRGB(
  const SCREEN*           this,
  size                    x,
  size                    y,
  const struct PixelColor color);

static void __WritePixelBGR(
  const SCREEN*           this,
  size                    x,
  size                    y,
  const struct PixelColor color);

static byte* __GetPixelAddress(
  const SCREEN* this,
  size          x,
  size          y);

#endif