#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "Fonts.h"
#include "Type.h"

typedef struct      __SCREEN SCREEN;

typedef void
    (*WRITE_PIXEL)(
      IN OUT const SCREEN*       this, 
      IN uint32_t                x, 
      IN uint32_t                y, 
      IN const struct PixelColor color);
typedef uint8_t*
    (*GET_PIXEL_ADDRESS)(
      IN const SCREEN* this, 
      IN uint32_t      x, 
      IN uint32_t      y);

enum PIXEL_FORMAT {
    pixelRGBReserved8BitPerColor,
    pixelBGRReserved8BitPerColor
};

struct FRAME_BUFFER_CONFIG {
    uint32_t          pixelsPerScanLine;
    uint32_t          horizontalResolution;
    uint32_t          verticalResolution;

    enum PIXEL_FORMAT pixelFormat;
    uint8_t*          frameBuffer;
};

struct __SCREEN {
    uint32_t          pixelsPerScanLine;
    uint32_t          horizontalResolution;
    uint32_t          verticalResolution;

    uint8_t*          frameBuffer;

    WRITE_PIXEL       WritePixel;
    GET_PIXEL_ADDRESS GetPixelAddress;
};

struct PixelColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void CreateScreen(
  OUT SCREEN* this,
  IN  const struct FRAME_BUFFER_CONFIG* frameBufferConfig);

static void __WritePixelRGB(
  IN OUT const SCREEN* this,
  IN     uint32_t                x,
  IN     uint32_t                y,
  IN     const struct PixelColor color);

static void __WritePixelBGR(
  IN OUT const SCREEN* this,
  IN     uint32_t                x,
  IN     uint32_t                y,
  IN     const struct PixelColor color);

static uint8_t* __GetPixelAddress(
  IN const SCREEN* this,
  IN uint32_t      x,
  IN uint32_t      y);

#endif