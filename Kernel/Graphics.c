#include "Graphics.h"

code CreateScreen(
  OUT SCREEN*                           this,
  IN  const struct FRAME_BUFFER_CONFIG* frameBufferConfig) {
    if (frameBufferConfig->pixelsPerScanLine == 0
      or frameBufferConfig->horizontalResolution == 0
      or frameBufferConfig->verticalResolution == 0) {
        return INVALID_PARAMETER;
    }

    if (frameBufferConfig->frameBuffer == NULL) {
        return MEMORY_ERROR;
    }

    this->pixelsPerScanLine = frameBufferConfig->pixelsPerScanLine;
    this->horizontalResolution = frameBufferConfig->horizontalResolution;
    this->verticalResolution = frameBufferConfig->verticalResolution;

    this->frameBuffer = frameBufferConfig->frameBuffer;

    SetMemory(this->frameBuffer, 0, this->horizontalResolution * this->verticalResolution * sizeof(byte) * 3);

    if (frameBufferConfig->pixelFormat == pixelRGBReserved8BitPerColor) {
        this->WritePixel = __WritePixelRGB;
    }
    else {
        this->WritePixel = __WritePixelBGR;
    }
    
    this->GetPixelAddress = __GetPixelAddress;

    return SUCCESS;
}

static code __WritePixelRGB(
  IN OUT const SCREEN*           this,
  IN     size                    x,
  IN     size                    y,
  IN     const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(
      this, 
      x, 
      y);

    pixel[0] = color.red;
    pixel[1] = color.green;
    pixel[2] = color.blue;

    return SUCCESS;
}

static code __WritePixelBGR(
    IN OUT const SCREEN*           this,
    IN     size                    x,
    IN     size                    y,
    IN     const struct PixelColor color) {
    byte* pixel = this->GetPixelAddress(
      this, 
      x, 
      y);

    pixel[0] = color.blue;
    pixel[1] = color.green;
    pixel[2] = color.red;

    return SUCCESS;
}

static byte* __GetPixelAddress(
  IN const SCREEN* this,
  IN size          x,
  IN size          y) {
    return this->frameBuffer + 4 * (this->pixelsPerScanLine * y + x);
}