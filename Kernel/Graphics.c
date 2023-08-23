#include "Graphics.h"

void CreateScreen(
  OUT SCREEN*                           this,
  IN  const struct FRAME_BUFFER_CONFIG* frameBufferConfig) {
    this->pixelsPerScanLine = frameBufferConfig->pixelsPerScanLine;
    this->horizontalResolution = frameBufferConfig->horizontalResolution;
    this->verticalResolution = frameBufferConfig->verticalResolution;

    this->frameBuffer = frameBufferConfig->frameBuffer;

    if (frameBufferConfig->pixelFormat == pixelRGBReserved8BitPerColor) {
        this->WritePixel = __WritePixelRGB;
    }
    else {
        this->WritePixel = __WritePixelBGR;
    }
    
    this->GetPixelAddress = __GetPixelAddress;
}

static void __WritePixelRGB(
  IN OUT const SCREEN*           this,
  IN     uint32_t                x,
  IN     uint32_t                y,
  IN     const struct PixelColor color) {
    uint8_t* Pixel = this->GetPixelAddress(
      this, 
      x, 
      y);

    Pixel[0] = color.red;
    Pixel[1] = color.green;
    Pixel[2] = color.blue;
}

static void __WritePixelBGR(
    IN OUT const SCREEN*           this,
    IN     uint32_t                x,
    IN     uint32_t                y,
    IN     const struct PixelColor color) {
    uint8_t* Pixel = this->GetPixelAddress(
      this, 
      x, 
      y);

    Pixel[0] = color.blue;
    Pixel[1] = color.green;
    Pixel[2] = color.red;
}

static uint8_t* __GetPixelAddress(
  IN const SCREEN* this,
  IN uint32_t      x,
  IN uint32_t      y) {
    return this->frameBuffer + 4 * (this->pixelsPerScanLine * y + x);
}