#include "Main.h"

void Main(const struct FRAMEBUFFER_CONFIG* FrameBufferConfig) {
    uint32_t x, y;

    for (x = 0; x < (*FrameBufferConfig).horizontalResolution; ++x) {
        for (y = 0; y < (*FrameBufferConfig).verticalResolution; ++y) {
            WritePixel(FrameBufferConfig, x, y, (struct PixelColor){ 255, 255, 255 });
        }
    }

    for (x = 0; x < 200; ++x) {
        for (y = 0; y < 100; ++y) {
            WritePixel(FrameBufferConfig, 100 + x, 100 + y, (struct PixelColor){ 0, 255, 0 });
        }
    }
    
    while (1) { Halt(); }
}

int WritePixel(
  const struct FRAMEBUFFER_CONFIG* FrameBufferConfig, 
  uint32_t x,
  uint32_t y,
  const struct PixelColor Color) {
    const uint32_t pixelPosition = (*FrameBufferConfig).pixelsPerScanLine * y + x;

    if (FrameBufferConfig->PixelFormat == PixelRGBReserved8BitPerColor) {
        uint8_t* Pixel = &FrameBufferConfig->frameBuffer[4 * pixelPosition];
        Pixel[0] = Color.red;
        Pixel[1] = Color.green;
        Pixel[2] = Color.blue;
    } 
    else if (FrameBufferConfig->PixelFormat == PixelBGRReserved8BitPerColor) {
        uint8_t* Pixel = &FrameBufferConfig->frameBuffer[4 * pixelPosition];
        Pixel[0] = Color.blue;
        Pixel[1] = Color.green;
        Pixel[2] = Color.red;
    }
    else {
        return -1;
    }

    return 0;
}