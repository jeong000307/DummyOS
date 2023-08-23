#ifndef __MAIN_H__
#define __MAIN_H__

#include "Type.h"

enum PixelFormat {
    PixelRGBReserved8BitPerColor,
    PixelBGRReserved8BitPerColor
};

struct FRAMEBUFFER_CONFIG {
    uint32_t            pixelsPerScanLine;
    uint32_t            horizontalResolution;
    uint32_t            verticalResolution;

    enum PIXEL_FORMAT   PixelFormat;
    uint8_t*            frameBuffer;
};

struct PixelColor {
    uint8_t red, green, blue;
};

extern void Halt(void);

int WritePixel(
  const struct FRAMEBUFFER_CONFIG* FrameBufferConfig,
  uint32_t x,
  uint32_t y,
  const struct PixelColor Color);

#endif
