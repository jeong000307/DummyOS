#include "Main.h"

void Main(const struct FRAMEBUFFER_CONFIG* frameBufferConfig) {
    uint32_t x, y;

    for (x = 0; x < (*frameBufferConfig).horizontalResolution; ++x) {
        for (y = 0; y < (*frameBufferConfig).verticalResolution; ++y) {
            WritePixel(frameBufferConfig, x, y, (struct PixelColor){ 255, 255, 255 });
        }
    }

    for (x = 0; x < 200; ++x) {
        for (y = 0; y < 100; ++y) {
            WritePixel(frameBufferConfig, 100 + x, 100 + y, (struct PixelColor){ 0, 255, 0 });
        }
    }
    
    Halt();
}

int WritePixel(
  const struct FRAMEBUFFER_CONFIG* frameBufferConfig, 
  uint32_t x,
  uint32_t y,
  const struct PixelColor color) {
    const uint32_t pixelPosition = (*frameBufferConfig).pixelsPerScanLine * y + x;

    if (frameBufferConfig->pixelFormat == pixelRGBReserved8BitPerColor) {
        uint8_t* Pixel = &frameBufferConfig->frameBuffer[4 * pixelPosition];
        Pixel[0] = color.red;
        Pixel[1] = color.green;
        Pixel[2] = color.blue;
    } 
    else if (frameBufferConfig->pixelFormat == pixelBGRReserved8BitPerColor) {
        uint8_t* Pixel = &frameBufferConfig->frameBuffer[4 * pixelPosition];
        Pixel[0] = color.blue;
        Pixel[1] = color.green;
        Pixel[2] = color.red;
    }
    else {
        return -1;
    }

    return 0;
}