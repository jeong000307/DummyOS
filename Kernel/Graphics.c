#include "Graphics.h"

int32_t WritePixel(
    const struct FRAMEBUFFER_CONFIG* frameBufferConfig,
    uint32_t x,
    uint32_t y,
    const struct PixelColor color) {
    if (frameBufferConfig->pixelFormat == pixelRGBReserved8BitPerColor) {
        uint8_t* Pixel = GetPixelLocation(frameBufferConfig, x, y);
        Pixel[0] = color.red;
        Pixel[1] = color.green;
        Pixel[2] = color.blue;
    }
    else if (frameBufferConfig->pixelFormat == pixelBGRReserved8BitPerColor) {
        uint8_t* Pixel = GetPixelLocation(frameBufferConfig, x, y);
        Pixel[0] = color.blue;
        Pixel[1] = color.green;
        Pixel[2] = color.red;
    }
    else {
        return -1;
    }

    return 0;
}

uint8_t* GetPixelLocation(const struct FRAMEBUFFER_CONFIG* frameBufferConfig, uint32_t x, uint32_t y) {
    return frameBufferConfig->frameBuffer + 4 * (frameBufferConfig->pixelsPerScanLine * y + x);
}

const char* GetFont(char character) {
    uint32_t index = 16 * (uint32_t)character;

    return fonts + index;
}

void WriteAscii(const struct FRAMEBUFFER_CONFIG* frameBufferConfig, uint32_t x, uint32_t y, char character, const struct PixelColor color) {
    uint32_t    dx, dy;
    const char* font = GetFont(character);

    for (dy = 0; dy < 16; ++dy) {
        for (dx = 0; dx < 8; ++dx) {
            if ((font[dy] << dx) & 0x80u) {
                WritePixel(frameBufferConfig, x + dx, y + dy, color);
            }
        }
    }
}

void WriteString(const struct FRAMEBUFFER_CONFIG* frameBufferConfig, uint32_t x, uint32_t y, const char* string, const struct PixelColor color) {
    uint32_t index;

    for (index = 0; string[index] != '\0'; ++index) {
        WriteAscii(frameBufferConfig, x + 8 * index, y, string[index], color);
    }
}

