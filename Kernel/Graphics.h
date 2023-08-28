#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "ErrorCode.h"
#include "Memory.h"
#include "Type.h"

typedef struct __SCREEN SCREEN;

typedef void (*WRITE_PIXEL)(
    IN OUT const SCREEN* this,
    IN size                    x,
    IN size                    y,
    IN const struct PixelColor color);

typedef byte* (*GET_PIXEL_ADDRESS)(
    IN const SCREEN* this,
    IN size          x,
    IN size          y);

struct PixelColor {
    byte red;
    byte green;
    byte blue;
};

enum PIXEL_FORMAT {
    pixelRGBReserved8BitPerColor,
    pixelBGRReserved8BitPerColor
};

struct FRAME_BUFFER_CONFIG {
    uint32            pixelsPerScanLine;
    uint32            horizontalResolution;
    uint32            verticalResolution;

    enum PIXEL_FORMAT pixelFormat;
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
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfig);

static void __WritePixelRGB(
  IN const SCREEN*           this,
  IN size                    x,
  IN size                    y,
  IN const struct PixelColor color);

static void __WritePixelBGR(
  IN const SCREEN*           this,
  IN size                    x,
  IN size                    y,
  IN const struct PixelColor color);

static byte* __GetPixelAddress(
  IN const SCREEN* this,
  IN size          x,
  IN size          y);

#endif