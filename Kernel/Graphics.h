#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "Fonts.h"
#include "Type.h"

enum PIXEL_FORMAT {
    pixelRGBReserved8BitPerColor,
    pixelBGRReserved8BitPerColor
};

struct FRAMEBUFFER_CONFIG {
    uint32_t            pixelsPerScanLine;
    uint32_t            horizontalResolution;
    uint32_t            verticalResolution;

    enum PIXEL_FORMAT   pixelFormat;
    uint8_t* frameBuffer;
};

struct PixelColor {
    uint8_t red, green, blue;
};

int32_t WritePixel(
    const struct FRAMEBUFFER_CONFIG* frameBufferConfig,
    uint32_t x,
    uint32_t y,
    const struct PixelColor color);

uint8_t* GetPixelLocation(const struct FRAMEBUFFER_CONFIG* frameBufferConfig, uint32_t x, uint32_t y);

const char* GetFont(char character);

void WriteAscii(const struct FRAMEBUFFER_CONFIG* frameBufferConfig, uint32_t x, uint32_t y, char character, const struct PixelColor color);

void WriteString(const struct FRAMEBUFFER_CONFIG* frameBufferConfig, uint32_t x, uint32_t y, const char* string, const struct PixelColor color);

#endif