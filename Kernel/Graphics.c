#include "Graphics.h"

static SCREEN screen;

SCREEN* GetScreen(void) {
    return &screen;
}

code InitializeScreen(
  IN const struct FRAME_BUFFER_CONFIG* frameBufferConfig) {
    if (frameBufferConfig->pixelsPerScanLine == 0
      or frameBufferConfig->horizontalResolution == 0
      or frameBufferConfig->verticalResolution == 0) {
        return INVALID_PARAMETER;
    }

    if (frameBufferConfig->frameBuffer == NULL) {
        return MEMORY_ERROR;
    }

    screen.frameBufferConfig.pixelFormat = frameBufferConfig->pixelFormat;
    screen.frameBufferConfig.frameBuffer = frameBufferConfig->frameBuffer;

    screen.frameBufferConfig.pixelsPerScanLine = frameBufferConfig->pixelsPerScanLine;
    screen.frameBufferConfig.horizontalResolution = frameBufferConfig->horizontalResolution;
    screen.frameBufferConfig.verticalResolution = frameBufferConfig->verticalResolution;

    SetMemory(
      screen.frameBufferConfig.frameBuffer,
      0, 
      (size)screen.frameBufferConfig.horizontalResolution * (size)screen.frameBufferConfig.verticalResolution * sizeof(byte) * 4);

    if (frameBufferConfig->pixelFormat == pixelRGBReserved8BitPerColor) {
        screen.WritePixel = __WritePixelRGB;
    }
    else {
        screen.WritePixel = __WritePixelBGR;
    }
    
    screen.GetPixelAddress = __GetPixelAddress;

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
    return this->frameBufferConfig.frameBuffer + 4 * sizeof(byte) * (this->frameBufferConfig.pixelsPerScanLine * y + x);
}